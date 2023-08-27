# GradeVault

A terminal-based student grade management system written in C. Enter student
names and marks, GradeVault calculates averages, assigns letter grades, flags
pass/fail/borderline status, and saves everything to a file so records
persist across sessions.

## X Factor

Most grade calculators just calculate and forget. GradeVault auto-generates a
**brutally honest performance summary** at the end of a session — it reads
like a report card, not a spreadsheet dump:

```
⚠️ Class Average: 51.9 - Below passing threshold
🔴 1 students failing | 🟡 1 borderline | 🟢 1 passing
📉 Weakest subject: Physics (avg 43.0)
📈 Strongest subject: Chemistry (avg 57.7)
(No previous batch on record yet - this is the baseline.)
A few students are sinking. Do not ignore them.
```

The summary is a real computed feature, not decorative text:

- **Weakest / strongest subject** is computed by averaging every mark
  recorded for each subject name across all students and picking the min/max.
- **Pass / borderline / fail counts** are computed per-student from each
  student's own average against fixed thresholds.
- **Trend vs. last batch** is computed by persisting the class average from
  the last time `summary` was run (in `gradevault_history.dat`) and diffing
  it against the current run's average, so GradeVault literally tells you
  "average dropped/improved N points from last batch."
- The closing verdict line (e.g. *"A few students are sinking. Do not ignore
  them."*) is chosen from the actual fail/borderline/pass counts, not a
  static string.

Terminal output is also color-coded with raw ANSI escape codes — green for
passing, yellow for borderline, red for failing — which is uncommon to see
done by hand in a plain C project (no ncurses, no external libraries).

## Key concepts demonstrated

- **C structs** — `Student` holds a name, a variable number of subjects, and
  a parallel array of per-subject marks.
- **File I/O** — `fopen`/`fprintf`/`fscanf` persist the roster to
  `gradevault.dat` (pipe-delimited text) and the last class average to
  `gradevault_history.dat`, both reloaded on the next run.
- **ANSI escape codes** — `\033[31m` / `\033[32m` / `\033[33m` / `\033[0m`
  drive red/green/yellow/reset terminal coloring without any library.
- **Computed summary statistics** — class average, per-subject averages,
  pass/borderline/fail tallies, and batch-over-batch trend, all computed at
  runtime from the in-memory roster.
- **Data persistence without a database** — a flat text file is the entire
  storage layer; no SQLite, no external dependencies.

## Commands

```
add "Name" Subject=Mark [Subject=Mark ...]   e.g. add "Akshat" Math=88 Physics=76 Chemistry=91 English=83
add "Name" Mark [Mark ...]                    e.g. add "Akshat" 88 76 91 83
                                               (bare marks are auto-labeled Math, Physics,
                                                Chemistry, English, Biology, CS, in that order)
view all                                      list every student with average, grade, and marks
summary                                       print the brutally honest class report card
export <file>                                 write the roster + summary to a plain-text file
help                                          show the command list
exit | quit                                   save the roster and quit
```

Grading scale: A ≥ 90, B ≥ 75, C ≥ 60, D ≥ 40, F < 40.
Status: PASS ≥ 50, BORDERLINE 40–49, FAIL < 40.
Class average below 60 is flagged as a warning.

## Sample session

```
$ ./gradevault
GradeVault - student grade management
Type 'help' for commands. 0 student record(s) loaded from gradevault.dat.
> add "Akshat" 88 76 91 83
Added Akshat - average 84.5, grade B [PASS]
> view all
1 student(s) on record:
Akshat          avg=84.5   grade=B  [PASS      ] marks: Math=88, Physics=76, Chemistry=91, English=83
> summary
⚠️  Class Average: 84.5 - Class is passing overall
🔴 0 students failing | 🟡 0 borderline | 🟢 1 passing
📉 Weakest subject: Physics (avg 76.0)
📈 Strongest subject: Chemistry (avg 91.0)
(No previous batch on record yet - this is the baseline.)
Solid batch - everyone is clearing the bar.
> export report.txt
Exported report to report.txt
> exit
Saved 1 student record(s) to gradevault.dat. Goodbye.
```

Relaunching `./gradevault` reloads `gradevault.dat`, so the roster and the
"last batch" average both carry over into the next session.

## Build & run

Platform: Linux/macOS (GCC), Windows (MinGW).

```sh
gcc gradevault.c -o gradevault -Wall
./gradevault
```

On Windows with MinGW:

```sh
gcc gradevault.c -o gradevault.exe -Wall
gradevault.exe
```

## Files

- `gradevault.c` — the entire program (single file, no dependencies beyond
  the C standard library).
- `gradevault.dat` — generated at runtime; the persisted student roster.
- `gradevault_history.dat` — generated at runtime; last recorded class
  average, used for the batch-over-batch trend line.
- Both `.dat` files and any exported report files are gitignored since they
  are runtime state, not source.

## Notes

Built as a focused, single-purpose tool - a terminal-based student grade management system, nothing more, nothing less.

## Troubleshooting

If something doesn't run as expected, double-check you're using the dependency versions noted above and running the exact commands from the "Run it" section.

## Possible Improvements

- More test coverage
- Better error messages for edge cases
- A cleaner CLI/UI polish pass

## Acknowledgements

Thanks to the open-source libraries this project leans on - see the dependency list above for the full set.

## License

GPLv3 - see [LICENSE](LICENSE) for details.

## Testing

Manually tested via the sample commands above; no automated test suite yet.

## Notes

Built as a focused, single-purpose tool - a terminal-based student grade management system, nothing more, nothing less.

## Troubleshooting

If something doesn't run as expected, double-check you're using the dependency versions noted above and running the exact commands from the "Run it" section.

## Possible Improvements

- More test coverage
- Better error messages for edge cases
- A cleaner CLI/UI polish pass

## Acknowledgements

Thanks to the open-source libraries this project leans on - see the dependency list above for the full set.

## License

GPLv3 - see [LICENSE](LICENSE) for details.

## Testing

Manually tested via the sample commands above; no automated test suite yet.
