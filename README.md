<p align="center">
  <svg xmlns="http://www.w3.org/2000/svg" width="64" height="64" viewBox="0 0 24 24"
       fill="none" stroke="#4ade80" stroke-width="1.5"
       stroke-linecap="round" stroke-linejoin="round">
    <path d="M21.42 10.922a1 1 0 0 0-.019-1.838L12.83 5.18a2 2 0 0 0-1.66 0L2.6 9.08a1 1 0 0 0 0 1.832l8.57 3.908a2 2 0 0 0 1.66 0z"/>
    <path d="M22 10v6"/>
    <path d="M6 12.5V16a6 3 0 0 0 12 0v-3.5"/>
  </svg>
</p>

<h1 align="center">GradeVault</h1>

<p align="center">
  <strong>A terminal grade manager in plain C that stores records in a file and writes a brutally honest class report card.</strong>
</p>

<p align="center">
  <a href="https://gradevault.akshatgcodes.in">
    <img src="https://img.shields.io/badge/project_page-4ade80-4ade80?style=flat-square" alt="Project page">
  </a>
  <img src="https://img.shields.io/badge/C-A8B9CC?style=flat-square&logo=c&logoColor=black" alt="C">
  <img src="https://img.shields.io/badge/no_dependencies-111111?style=flat-square" alt="No dependencies">
  <img src="https://img.shields.io/badge/license-GPL--3.0-4ade80?style=flat-square" alt="License">
</p>

---

## What is GradeVault?

GradeVault is a command-line tool for teachers and TAs. Type students and marks, and it works out averages, letter grades and pass / borderline / fail status, then keeps everything in a flat file between sessions. Its `summary` command reads like a report card, not a spreadsheet dump: weakest and strongest subject, how many students are sinking, and whether the class average went up or down since the last batch.

> **Project page →** [gradevault.akshatgcodes.in](https://gradevault.akshatgcodes.in)
> It's a CLI, so it runs on your machine: `gcc -Wall gradevault.c -o gradevault && ./gradevault`

```
> summary
⚠️  Class Average: 51.9 - Below passing threshold
🔴 1 students failing | 🟡 1 borderline | 🟢 1 passing
📉 Weakest subject: Physics (avg 43.0)
📈 Strongest subject: Chemistry (avg 57.7)
A few students are sinking. Do not ignore them.
```

---

## What you get

- **Records that persist** — `add`, `view all`, and everything is saved to `gradevault.dat` and reloaded next time.
- **Grades and status** — A ≥ 90, B ≥ 75, C ≥ 60, D ≥ 40, F below. PASS ≥ 50, BORDERLINE 40–49, FAIL below 40.
- **Honest summary** — per-subject averages, pass/borderline/fail counts, a trend against the last batch, and a verdict chosen from the real counts.
- **Colour output** — green, yellow and red status with raw ANSI escape codes, no ncurses.
- **Export** — write the roster and summary to a text file.

---

## Stack

| Layer | Tech |
|---|---|
| Language | C (standard library only) |
| Storage | Pipe-delimited text files (`gradevault.dat`, `gradevault_history.dat`) |
| Output | ANSI escape codes |
| Project page | Next.js 16 on Vercel · GoDaddy DNS |

---

## Engineering Decisions

**Why a flat text file instead of SQLite?**
The tool stays one `.c` file with zero dependencies. A pipe-delimited roster is enough for a class and is easy to read or fix by hand.

**Why store the last class average separately?**
`gradevault_history.dat` keeps the average from the previous `summary`, so the next run can say "dropped / improved N points from last batch" without keeping full history.

**Why fixed-size arrays?**
200 students × 10 subjects covers a class, keeps memory static and avoids `malloc` bookkeeping. Every string copy is length-bounded.

**What would you do differently in v2?**
Quote-aware CSV import and export, so a roster can come straight from a spreadsheet.

---

## Docs

| Document | Description |
|---|---|
| [PRD](docs/PRD.md) | Product requirements — goals, user stories, non-goals |
| [Architecture](docs/ARCHITECTURE.md) | Program structure, file formats, data flow |
| [Decisions](docs/DECISIONS.md) | Every major technical decision and why |
| [Setup](docs/SETUP.md) | Build, run and every command |
| [Deploy](docs/DEPLOY.md) | How the project page is deployed |
| [Contributing](CONTRIBUTING.md) | How to propose changes |
| [Changelog](CHANGELOG.md) | What changed in each release |
| [Security](SECURITY.md) | How to report a vulnerability |

---

<div align="center">

<h3>Akshat Gupta</h3>

<a href="https://akshatgcodes.in"><img src="https://img.shields.io/badge/Website-akshatgcodes.in-111111?style=flat-square&logo=googlechrome&logoColor=white" alt="Website"></a>
<a href="https://linkedin.com/in/akshatgcodes"><img src="https://img.shields.io/badge/LinkedIn-akshatgcodes-0A66C2?style=flat-square" alt="LinkedIn"></a>
<a href="https://github.com/akshatgcodes"><img src="https://img.shields.io/badge/GitHub-akshatgcodes-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub"></a>
<a href="mailto:akshatgcodes@gmail.com"><img src="https://img.shields.io/badge/Email-akshatgcodes%40gmail.com-EA4335?style=flat-square&logo=gmail&logoColor=white" alt="Email"></a>

</div>
