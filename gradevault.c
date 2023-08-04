/*
 * GradeVault - a terminal-based student grade management system.
 *
 * Enter student names and per-subject marks, GradeVault computes averages,
 * assigns letter grades, and flags pass/fail.
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

#define PASS_THRESHOLD    50.0   /* overall average needed to pass       */
#define BORDERLINE_FLOOR  40.0   /* below this and above FAIL is border. */

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
 * Commands
 * --------------------------------------------------------------------- */

static void print_help(void) {
    printf("Available commands:\n");
    printf("  add \"Name\" Subject=Mark [Subject=Mark ...]   e.g. add \"Akshat\" Math=88 Physics=76\n");
    printf("  add \"Name\" Mark [Mark ...]                    e.g. add \"Akshat\" 88 76 91 83\n");
    printf("  view all                                       list every student and their grade\n");
    printf("  help                                            show this message\n");
    printf("  exit | quit                                     quit\n");
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

static void print_student_row(const Student *s) {
    double avg = student_average(s);
    char grade = letter_grade(avg);
    int status = student_status(avg);

    printf("%s%-15s%s avg=%-6.1f grade=%-2c [%s%-10s%s] marks: ",
           C_BOLD, s->name, C_RESET, avg, grade,
           status_color(status), status_label(status), C_RESET);

    for (int i = 0; i < s->num_subjects; i++) {
        printf("%s=%.0f", s->subject_names[i], s->marks[i]);
        if (i < s->num_subjects - 1) printf(", ");
    }
    printf("\n");
}

static void cmd_view_all(void) {
    if (student_count == 0) {
        printf(C_YELLOW "No students on record yet. Try: add \"Name\" 88 76 91 83\n" C_RESET);
        return;
    }
    printf(C_CYAN "%d student(s) on record:\n" C_RESET, student_count);
    for (int i = 0; i < student_count; i++) {
        print_student_row(&students[i]);
    }
}

/* ---------------------------------------------------------------------
 * Main REPL
 * --------------------------------------------------------------------- */

int main(void) {
    printf(C_BOLD "GradeVault" C_RESET " - student grade management\n");
    printf("Type 'help' for commands.\n");

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
        } else if (strcmp(cmd, "help") == 0) {
            print_help();
        } else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
            break;
        } else {
            printf(C_YELLOW "Unknown command '%s'. Type 'help' for a list.\n" C_RESET, cmd);
        }
    }

    printf("Goodbye.\n");
    return 0;
}
