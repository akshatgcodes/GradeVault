/*
 * GradeVault - a terminal-based student grade management system.
 *
 * Enter student names and per-subject marks, GradeVault computes averages,
 * assigns letter grades, flags pass/fail, colorizes the output with ANSI
 * escape codes, and persists everything to a data file so records survive
 * across sessions. The "summary" command generates a blunt, human-readable
 * report card for the whole class instead of a spreadsheet dump.
 *
 * Build:   gcc gradevault.c -o gradevault -Wall
 * Run:     ./gradevault
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS      200
#define MAX_SUBJECTS      10
#define MAX_NAME_LEN      50
#define MAX_SUBJECT_LEN   30
#define LINE_LEN          1024

#define DATA_FILE         "gradevault.dat"
#define HISTORY_FILE      "gradevault_history.dat"

#define PASS_THRESHOLD    50.0   /* overall average needed to pass       */
#define BORDERLINE_FLOOR  40.0   /* below this and above FAIL is border. */
#define CLASS_WARN_LEVEL  60.0   /* class average below this is a warning*/

/* ---- ANSI colors ---------------------------------------------------- */
#define C_RESET   "\033[0m"
#define C_RED     "\033[31m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_BOLD    "\033[1m"
#define C_CYAN    "\033[36m"

typedef struct {
    char name[MAX_NAME_LEN];
    int  num_subjects;
    char subject_names[MAX_SUBJECTS][MAX_SUBJECT_LEN];
    double marks[MAX_SUBJECTS];
} Student;

static Student students[MAX_STUDENTS];
static int student_count = 0;

static const char *DEFAULT_SUBJECTS[] = {
    "Math", "Physics", "Chemistry", "English", "Biology", "CS"
};
#define NUM_DEFAULT_SUBJECTS (int)(sizeof(DEFAULT_SUBJECTS) / sizeof(DEFAULT_SUBJECTS[0]))

/* ---------------------------------------------------------------------
 * Small helpers
 * --------------------------------------------------------------------- */

static double student_average(const Student *s) {
    if (s->num_subjects == 0) return 0.0;
    double total = 0.0;
    for (int i = 0; i < s->num_subjects; i++) total += s->marks[i];
    return total / s->num_subjects;
}

static char letter_grade(double avg) {
    if (avg >= 90) return 'A';
    if (avg >= 75) return 'B';
    if (avg >= 60) return 'C';
    if (avg >= BORDERLINE_FLOOR) return 'D';
    return 'F';
}

/* status: 0 = fail, 1 = borderline, 2 = pass */
static int student_status(double avg) {
    if (avg < BORDERLINE_FLOOR) return 0;
    if (avg < PASS_THRESHOLD) return 1;
    return 2;
}

static const char *status_color(int status) {
    switch (status) {
        case 0: return C_RED;
        case 1: return C_YELLOW;
        default: return C_GREEN;
    }
}

static const char *status_label(int status) {
    switch (status) {
        case 0: return "FAIL";
        case 1: return "BORDERLINE";
        default: return "PASS";
    }
}

static void trim_newline(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

/* ---------------------------------------------------------------------
 * Persistence (fopen / fprintf / fscanf)
 *
 * Record format, one line per student:
 *   name|num_subjects|subj1:mark1,subj2:mark2,...
 * ------------------------------------------------------------------- */

static void load_students(void) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) return; /* no prior data yet - that's fine */

    char line[LINE_LEN];
    while (fgets(line, sizeof(line), fp) && student_count < MAX_STUDENTS) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        Student *s = &students[student_count];
        memset(s, 0, sizeof(Student));

        char *name_end = strchr(line, '|');
        if (!name_end) continue;
        size_t name_len = (size_t)(name_end - line);
        if (name_len >= MAX_NAME_LEN) name_len = MAX_NAME_LEN - 1;
        strncpy(s->name, line, name_len);
        s->name[name_len] = '\0';

        char *rest = name_end + 1;
        int n = 0;
        char subj_field[LINE_LEN];
        if (sscanf(rest, "%d|%[^\n]", &n, subj_field) < 1) continue;
        if (n < 0) n = 0;
        if (n > MAX_SUBJECTS) n = MAX_SUBJECTS;
        s->num_subjects = n;

        /* find the field after the second '|' to parse subject:mark pairs */
        char *second_bar = strchr(rest, '|');
        if (second_bar && n > 0) {
            char *cursor = second_bar + 1;
            char *token = strtok(cursor, ",");
            int idx = 0;
            while (token && idx < n) {
                char subj[MAX_SUBJECT_LEN];
                double mark;
                if (sscanf(token, "%29[^:]:%lf", subj, &mark) == 2) {
                    strncpy(s->subject_names[idx], subj, MAX_SUBJECT_LEN - 1);
                    s->marks[idx] = mark;
                    idx++;
                }
                token = strtok(NULL, ",");
            }
            s->num_subjects = idx;
        } else {
            s->num_subjects = 0;
        }

        student_count++;
    }
    fclose(fp);
}

static void save_students(void) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) {
        printf(C_RED "Error: could not save records to %s\n" C_RESET, DATA_FILE);
        return;
    }
    for (int i = 0; i < student_count; i++) {
        Student *s = &students[i];
        fprintf(fp, "%s|%d|", s->name, s->num_subjects);
        for (int j = 0; j < s->num_subjects; j++) {
            fprintf(fp, "%s:%.2f", s->subject_names[j], s->marks[j]);
            if (j < s->num_subjects - 1) fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/* previous class average, -1.0 means "no history yet" */
static double load_history(void) {
    FILE *fp = fopen(HISTORY_FILE, "r");
    if (!fp) return -1.0;
    double prev = -1.0;
    if (fscanf(fp, "%lf", &prev) != 1) prev = -1.0;
    fclose(fp);
    return prev;
}

static void save_history(double avg) {
    FILE *fp = fopen(HISTORY_FILE, "w");
    if (!fp) return;
    fprintf(fp, "%.4f\n", avg);
    fclose(fp);
}

/* ---------------------------------------------------------------------
 * Commands
 * --------------------------------------------------------------------- */

static void print_help(void) {
    printf("Available commands:\n");
    printf("  add \"Name\" Subject=Mark [Subject=Mark ...]   e.g. add \"Akshat\" Math=88 Physics=76\n");
    printf("  add \"Name\" Mark [Mark ...]                    e.g. add \"Akshat\" 88 76 91 83\n");
    printf("  view all                                       list every student and their grade\n");
    printf("  summary                                        brutally honest class report card\n");
    printf("  export <file>                                  write the roster + summary to a file\n");
    printf("  help                                            show this message\n");
    printf("  exit | quit                                     save and quit\n");
}

/* Parses:  "Name" tok1 tok2 ...   where tok is either Subject=Mark or a bare number */
static void cmd_add(char *args) {
    while (*args == ' ') args++;

    if (*args != '"') {
        printf(C_RED "Error: student name must be quoted, e.g. add \"Akshat\" 88 76\n" C_RESET);
        return;
    }
    args++;
    char *name_end = strchr(args, '"');
    if (!name_end) {
        printf(C_RED "Error: missing closing quote around name\n" C_RESET);
        return;
    }
    if (student_count >= MAX_STUDENTS) {
        printf(C_RED "Error: student roster is full (%d max)\n" C_RESET, MAX_STUDENTS);
        return;
    }

    Student s;
    memset(&s, 0, sizeof(Student));
    size_t name_len = (size_t)(name_end - args);
    if (name_len == 0) {
        printf(C_RED "Error: student name cannot be empty\n" C_RESET);
        return;
    }
    if (name_len >= MAX_NAME_LEN) name_len = MAX_NAME_LEN - 1;
    strncpy(s.name, args, name_len);
    s.name[name_len] = '\0';

    char *rest = name_end + 1;
    int default_idx = 0;
    char *token = strtok(rest, " \t");
    while (token && s.num_subjects < MAX_SUBJECTS) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            char *subj = token;
            char *mark_str = eq + 1;
            double mark = atof(mark_str);
            strncpy(s.subject_names[s.num_subjects], subj, MAX_SUBJECT_LEN - 1);
            s.marks[s.num_subjects] = mark;
            s.num_subjects++;
        } else {
            /* bare numeric mark - assign a default subject name in order */
            double mark = atof(token);
            const char *subj_name;
            if (default_idx < NUM_DEFAULT_SUBJECTS) {
                subj_name = DEFAULT_SUBJECTS[default_idx];
            } else {
                static char fallback[MAX_SUBJECT_LEN];
                snprintf(fallback, sizeof(fallback), "Subject%d", default_idx + 1);
                subj_name = fallback;
            }
            strncpy(s.subject_names[s.num_subjects], subj_name, MAX_SUBJECT_LEN - 1);
            s.marks[s.num_subjects] = mark;
            s.num_subjects++;
            default_idx++;
        }
        token = strtok(NULL, " \t");
    }

    if (s.num_subjects == 0) {
        printf(C_RED "Error: at least one mark is required, e.g. add \"Akshat\" 88 76 91 83\n" C_RESET);
        return;
    }

    students[student_count++] = s;
    double avg = student_average(&s);
    int status = student_status(avg);
    printf("Added %s%s%s - average %.1f, grade %c [%s%s%s]\n",
           C_BOLD, s.name, C_RESET, avg, letter_grade(avg),
           status_color(status), status_label(status), C_RESET);
}

static void print_student_row(FILE *out, const Student *s, int use_color) {
    double avg = student_average(s);
    char grade = letter_grade(avg);
    int status = student_status(avg);

    if (use_color) {
        printf("%s%-15s%s avg=%-6.1f grade=%-2c [%s%-10s%s] marks: ",
               C_BOLD, s->name, C_RESET, avg, grade,
               status_color(status), status_label(status), C_RESET);
    } else {
        fprintf(out, "%-15s avg=%-6.1f grade=%-2c [%-10s] marks: ",
                s->name, avg, grade, status_label(status));
    }

    for (int i = 0; i < s->num_subjects; i++) {
        if (use_color) {
            printf("%s=%.0f", s->subject_names[i], s->marks[i]);
            if (i < s->num_subjects - 1) printf(", ");
        } else {
            fprintf(out, "%s=%.0f", s->subject_names[i], s->marks[i]);
            if (i < s->num_subjects - 1) fprintf(out, ", ");
        }
    }
    if (use_color) printf("\n"); else fprintf(out, "\n");
}

static void cmd_view_all(void) {
    if (student_count == 0) {
        printf(C_YELLOW "No students on record yet. Try: add \"Name\" 88 76 91 83\n" C_RESET);
        return;
    }
    printf(C_CYAN "%d student(s) on record:\n" C_RESET, student_count);
    for (int i = 0; i < student_count; i++) {
        print_student_row(NULL, &students[i], 1);
    }
}

/* Computes and prints/writes the "brutally honest" summary.
 * If out is NULL, prints to stdout with color. Otherwise writes plain
 * text to out (used by export). Updates history file as a side effect
 * only when called from the interactive "summary" command (update_history=1).
 */
static void run_summary(FILE *out, int use_color, int update_history) {
    if (student_count == 0) {
        if (use_color) printf(C_YELLOW "No students on record yet - nothing to summarize.\n" C_RESET);
        else fprintf(out, "No students on record yet - nothing to summarize.\n");
        return;
    }

    double class_total = 0.0;
    int fail_count = 0, borderline_count = 0, pass_count = 0;

    /* per-subject totals, tracked by first-seen subject name */
    char subj_names[MAX_SUBJECTS * MAX_STUDENTS][MAX_SUBJECT_LEN];
    double subj_totals[MAX_SUBJECTS * MAX_STUDENTS];
    int subj_counts[MAX_SUBJECTS * MAX_STUDENTS];
    int subj_unique = 0;

    for (int i = 0; i < student_count; i++) {
        Student *s = &students[i];
        double avg = student_average(s);
        class_total += avg;
        int status = student_status(avg);
        if (status == 0) fail_count++;
        else if (status == 1) borderline_count++;
        else pass_count++;

        for (int j = 0; j < s->num_subjects; j++) {
            int found = -1;
            for (int k = 0; k < subj_unique; k++) {
                if (strcmp(subj_names[k], s->subject_names[j]) == 0) { found = k; break; }
            }
            if (found == -1) {
                found = subj_unique++;
                strncpy(subj_names[found], s->subject_names[j], MAX_SUBJECT_LEN - 1);
                subj_names[found][MAX_SUBJECT_LEN - 1] = '\0';
                subj_totals[found] = 0.0;
                subj_counts[found] = 0;
            }
            subj_totals[found] += s->marks[j];
            subj_counts[found]++;
        }
    }

    double class_avg = class_total / student_count;

    int weakest_idx = -1;
    double weakest_avg = 1e18;
    int strongest_idx = -1;
    double strongest_avg = -1e18;
    for (int k = 0; k < subj_unique; k++) {
        double subj_avg = subj_totals[k] / subj_counts[k];
        if (subj_avg < weakest_avg) { weakest_avg = subj_avg; weakest_idx = k; }
        if (subj_avg > strongest_avg) { strongest_avg = subj_avg; strongest_idx = k; }
    }

    double prev_avg = update_history ? load_history() : -1.0;

    /* ---- header line: class average vs. passing threshold ---- */
    if (use_color) {
        const char *warn_color = (class_avg < CLASS_WARN_LEVEL) ? C_RED : C_GREEN;
        const char *icon = (class_avg < CLASS_WARN_LEVEL) ? "\xE2\x9A\xA0\xEF\xB8\x8F " : "\xE2\x9C\x85 ";
        printf("%s%sClass Average: %.1f", warn_color, icon, class_avg);
        if (class_avg < CLASS_WARN_LEVEL) printf(" - Below passing threshold");
        else printf(" - Class is passing overall");
        printf("%s\n", C_RESET);
    } else {
        fprintf(out, "Class Average: %.1f", class_avg);
        fprintf(out, class_avg < CLASS_WARN_LEVEL ? " - Below passing threshold\n" : " - Class is passing overall\n");
    }

    /* ---- pass/fail/borderline breakdown ---- */
    if (use_color) {
        printf("%s\xF0\x9F\x94\xB4 %d students failing%s | %s\xF0\x9F\x9F\xA1 %d borderline%s | %s\xF0\x9F\x9F\xA2 %d passing%s\n",
               C_RED, fail_count, C_RESET,
               C_YELLOW, borderline_count, C_RESET,
               C_GREEN, pass_count, C_RESET);
    } else {
        fprintf(out, "%d students failing | %d borderline | %d passing\n",
                fail_count, borderline_count, pass_count);
    }

    /* ---- weakest / strongest subject ---- */
    if (weakest_idx != -1) {
        if (use_color) {
            printf("%s\xF0\x9F\x93\x89 Weakest subject: %s (avg %.1f)%s\n",
                   C_RED, subj_names[weakest_idx], weakest_avg, C_RESET);
            printf("%s\xF0\x9F\x93\x88 Strongest subject: %s (avg %.1f)%s\n",
                   C_GREEN, subj_names[strongest_idx], strongest_avg, C_RESET);
        } else {
            fprintf(out, "Weakest subject: %s (avg %.1f)\n", subj_names[weakest_idx], weakest_avg);
            fprintf(out, "Strongest subject: %s (avg %.1f)\n", subj_names[strongest_idx], strongest_avg);
        }
    }

    /* ---- comparison to last batch ---- */
    if (prev_avg >= 0.0) {
        double diff = class_avg - prev_avg;
        if (use_color) {
            if (diff < -0.05) {
                printf("%s\xF0\x9F\x93\x89 Average dropped %.1f points from last batch (%.1f -> %.1f)%s\n",
                       C_RED, -diff, prev_avg, class_avg, C_RESET);
            } else if (diff > 0.05) {
                printf("%s\xF0\x9F\x93\x88 Average improved %.1f points from last batch (%.1f -> %.1f)%s\n",
                       C_GREEN, diff, prev_avg, class_avg, C_RESET);
            } else {
                printf("%s\xE2\x9E\x96 Average is flat compared to last batch (%.1f)%s\n", C_YELLOW, prev_avg, C_RESET);
            }
        } else {
            if (diff < -0.05)
                fprintf(out, "Average dropped %.1f points from last batch (%.1f -> %.1f)\n", -diff, prev_avg, class_avg);
            else if (diff > 0.05)
                fprintf(out, "Average improved %.1f points from last batch (%.1f -> %.1f)\n", diff, prev_avg, class_avg);
            else
                fprintf(out, "Average is flat compared to last batch (%.1f)\n", prev_avg);
        }
    } else {
        if (use_color) printf(C_CYAN "(No previous batch on record yet - this is the baseline.)\n" C_RESET);
        else fprintf(out, "(No previous batch on record yet - this is the baseline.)\n");
    }

    /* ---- blunt closing verdict ---- */
    const char *verdict;
    if (fail_count > pass_count) verdict = "Most of this class is failing. This batch needs serious intervention.";
    else if (fail_count > 0) verdict = "A few students are sinking. Do not ignore them.";
    else if (borderline_count > 0) verdict = "Nobody is failing, but several students are one bad test from it.";
    else verdict = "Solid batch - everyone is clearing the bar.";

    if (use_color) printf("%s%s%s\n", C_BOLD, verdict, C_RESET);
    else fprintf(out, "%s\n", verdict);

    if (update_history) save_history(class_avg);
}

static void cmd_export(char *filename) {
    while (*filename == ' ') filename++;
    if (*filename == '\0') {
        printf(C_RED "Error: usage: export <filename>\n" C_RESET);
        return;
    }
    char clean[256];
    strncpy(clean, filename, sizeof(clean) - 1);
    clean[sizeof(clean) - 1] = '\0';
    trim_newline(clean);

    FILE *fp = fopen(clean, "w");
    if (!fp) {
        printf(C_RED "Error: could not open %s for writing\n" C_RESET, clean);
        return;
    }

    fprintf(fp, "GradeVault Report\n");
    fprintf(fp, "==================\n\n");
    fprintf(fp, "Roster (%d student(s)):\n", student_count);
    for (int i = 0; i < student_count; i++) {
        print_student_row(fp, &students[i], 0);
    }
    fprintf(fp, "\nSummary:\n");
    run_summary(fp, 0, 0); /* export does not overwrite the "last batch" history */

    fclose(fp);
    printf(C_GREEN "Exported report to %s\n" C_RESET, clean);
}

/* ---------------------------------------------------------------------
 * Main REPL
 * --------------------------------------------------------------------- */

int main(void) {
    load_students();
    printf(C_BOLD "GradeVault" C_RESET " - student grade management\n");
    printf("Type 'help' for commands. %d student record(s) loaded from %s.\n",
           student_count, DATA_FILE);

    char line[LINE_LEN];
    for (;;) {
        printf("> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break; /* EOF, e.g. piped input ran out */
        }
        trim_newline(line);

        char *cmd = line;
        while (*cmd == ' ') cmd++;
        if (*cmd == '\0') continue;

        if (strncmp(cmd, "add ", 4) == 0) {
            cmd_add(cmd + 4);
        } else if (strcmp(cmd, "view all") == 0 || strcmp(cmd, "view") == 0) {
            cmd_view_all();
        } else if (strcmp(cmd, "summary") == 0) {
            run_summary(NULL, 1, 1);
        } else if (strncmp(cmd, "export ", 7) == 0) {
            cmd_export(cmd + 7);
        } else if (strcmp(cmd, "help") == 0) {
            print_help();
        } else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
            break;
        } else {
            printf(C_YELLOW "Unknown command '%s'. Type 'help' for a list.\n" C_RESET, cmd);
        }
    }

    save_students();
    printf("Saved %d student record(s) to %s. Goodbye.\n", student_count, DATA_FILE);
    return 0;
}

// Built incrementally - see git history for the development progression.
