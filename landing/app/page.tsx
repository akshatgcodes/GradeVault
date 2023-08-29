import Terminal from "./components/Terminal";
import Badge from "./components/Badge";
import CodeBlock from "./components/CodeBlock";
import FeatureCard from "./components/FeatureCard";

const CONCEPTS = [
  "C structs",
  "File I/O (fopen/fprintf/fscanf)",
  "ANSI escape codes",
  "Computed statistics",
  "Flat-file persistence",
  "No external dependencies",
  "GCC / C standard library",
];

export default function Home() {
  return (
    <div className="flex flex-1 flex-col items-center">
      {/* nav */}
      <header className="w-full border-b border-border">
        <div className="mx-auto flex w-full max-w-5xl items-center justify-between px-6 py-4">
          <span className="font-mono text-sm text-foreground/90">
            <span className="text-accent-green">{">_"}</span> GradeVault
          </span>
          <nav className="flex gap-5 font-mono text-xs text-muted">
            <a href="#demo" className="transition-colors hover:text-foreground">
              demo
            </a>
            <a href="#x-factor" className="transition-colors hover:text-foreground">
              x-factor
            </a>
            <a href="#run" className="transition-colors hover:text-foreground">
              run it
            </a>
          </nav>
        </div>
      </header>

      {/* hero */}
      <section className="w-full max-w-5xl px-6 pb-16 pt-20 sm:pt-28">
        <p className="mb-4 font-mono text-xs uppercase tracking-widest text-accent-green">
          Terminal Tool · Written in C
        </p>
        <h1 className="max-w-3xl text-4xl font-semibold leading-tight tracking-tight text-foreground sm:text-5xl">
          A grade book that tells your class the truth.
        </h1>
        <p className="mt-5 max-w-2xl text-lg leading-relaxed text-muted">
          GradeVault is a terminal-based student grade management system written
          in plain C. Enter student names and marks — it computes averages,
          assigns letter grades, flags pass / borderline / fail status, and
          persists every record to disk so the roster carries over between
          sessions.
        </p>
        <div className="mt-8 flex flex-wrap gap-3 font-mono text-sm">
          <a
            href="#demo"
            className="rounded-md bg-accent-green px-5 py-2.5 font-medium text-black transition-opacity hover:opacity-90"
          >
            See it in action
          </a>
          <a
            href="#run"
            className="rounded-md border border-border px-5 py-2.5 text-foreground/90 transition-colors hover:border-accent-green/60 hover:text-foreground"
          >
            Run it locally
          </a>
        </div>
      </section>

      {/* description */}
      <section className="w-full max-w-5xl px-6 pb-16">
        <div className="grid gap-4 sm:grid-cols-3">
          <FeatureCard title="Grades &amp; status">
            Computes each student&apos;s average, assigns a letter grade (A–F),
            and flags pass, borderline, or fail against fixed thresholds.
          </FeatureCard>
          <FeatureCard title="Persistence, no DB">
            Every session is saved to a plain pipe-delimited text file and
            reloaded automatically the next time GradeVault runs.
          </FeatureCard>
          <FeatureCard title="Colored terminal UI">
            Raw ANSI escape codes drive green / yellow / red output — no
            ncurses, no external libraries, just the C standard library.
          </FeatureCard>
        </div>
      </section>

      {/* x-factor */}
      <section id="x-factor" className="w-full max-w-5xl scroll-mt-20 px-6 pb-16">
        <p className="mb-2 font-mono text-xs uppercase tracking-widest text-accent-green">
          The X Factor
        </p>
        <h2 className="text-2xl font-semibold text-foreground sm:text-3xl">
          It doesn&apos;t just calculate. It editorializes.
        </h2>
        <p className="mt-4 max-w-3xl leading-relaxed text-muted">
          Most grade calculators stop at the spreadsheet dump. GradeVault
          auto-generates a{" "}
          <span className="text-foreground">brutally honest performance summary</span>{" "}
          at the end of a session — a real computed feature, not decorative
          text:
        </p>
        <div className="mt-6 grid gap-4 sm:grid-cols-2">
          <FeatureCard title="Weakest / strongest subject">
            Averages every mark recorded for each subject across all students
            and picks the min and max.
          </FeatureCard>
          <FeatureCard title="Pass / borderline / fail tallies">
            Computed per-student from each student&apos;s own average against
            fixed thresholds — not hardcoded buckets.
          </FeatureCard>
          <FeatureCard title="Trend vs. last batch">
            Persists the class average from the last summary run to
            gradevault_history.dat and diffs it against the current run, so it
            can literally say &quot;average dropped 8 points from last
            batch.&quot;
          </FeatureCard>
          <FeatureCard title="A verdict, not a template">
            The closing line — e.g. &quot;A few students are sinking. Do not
            ignore them.&quot; — is chosen from the actual fail / borderline /
            pass counts.
          </FeatureCard>
        </div>
      </section>

      {/* demo terminal */}
      <section id="demo" className="w-full max-w-5xl scroll-mt-20 px-6 pb-16">
        <p className="mb-2 font-mono text-xs uppercase tracking-widest text-accent-green">
          Sample session
        </p>
        <h2 className="mb-6 text-2xl font-semibold text-foreground sm:text-3xl">
          What running it actually looks like
        </h2>
        <div className="flex justify-center">
          <Terminal />
        </div>
      </section>

      {/* key concepts */}
      <section className="w-full max-w-5xl px-6 pb-16">
        <p className="mb-4 font-mono text-xs uppercase tracking-widest text-accent-green">
          Key concepts
        </p>
        <div className="flex flex-wrap gap-2.5">
          {CONCEPTS.map((c) => (
            <Badge key={c}>{c}</Badge>
          ))}
        </div>
      </section>

      {/* run it */}
      <section id="run" className="w-full max-w-5xl scroll-mt-20 px-6 pb-20">
        <p className="mb-2 font-mono text-xs uppercase tracking-widest text-accent-green">
          Run it
        </p>
        <h2 className="mb-6 text-2xl font-semibold text-foreground sm:text-3xl">
          Two lines, no dependencies
        </h2>
        <div className="grid gap-6 sm:grid-cols-2">
          <div>
            <p className="mb-2 font-mono text-xs text-muted">macOS / Linux (GCC)</p>
            <CodeBlock
              lines={["gcc gradevault.c -o gradevault -Wall", "./gradevault"]}
            />
          </div>
          <div>
            <p className="mb-2 font-mono text-xs text-muted">Windows (MinGW)</p>
            <CodeBlock
              lines={[
                "gcc gradevault.c -o gradevault.exe -Wall",
                "gradevault.exe",
              ]}
            />
          </div>
        </div>
        <p className="mt-6 max-w-2xl text-sm leading-relaxed text-muted">
          Grading scale: A ≥ 90, B ≥ 75, C ≥ 60, D ≥ 40, F &lt; 40. Status:
          PASS ≥ 50, BORDERLINE 40–49, FAIL &lt; 40. The full command
          reference, file layout, and grading logic are documented in{" "}
          <code className="rounded bg-background-elevated px-1.5 py-0.5 font-mono text-xs text-foreground/90">
            README.md
          </code>{" "}
          in the project root.
        </p>
      </section>

      {/* footer */}
      <footer className="w-full border-t border-border">
        <div className="mx-auto flex w-full max-w-5xl flex-col gap-1 px-6 py-8 font-mono text-xs text-muted sm:flex-row sm:items-center sm:justify-between">
          <span>GradeVault — single-file C, no dependencies</span>
          <span>Licensed under GPLv3</span>
        </div>
      </footer>
    </div>
  );
}
