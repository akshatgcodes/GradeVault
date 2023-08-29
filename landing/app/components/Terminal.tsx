function Prompt({ children }: { children: React.ReactNode }) {
  return (
    <div>
      <span className="text-accent-green">{"> "}</span>
      <span className="text-foreground">{children}</span>
    </div>
  );
}

function Out({ children, className = "" }: { children: React.ReactNode; className?: string }) {
  return <div className={`text-muted ${className}`}>{children}</div>;
}

export default function Terminal() {
  return (
    <div className="w-full max-w-3xl overflow-hidden rounded-xl border border-border bg-background-elevated shadow-2xl shadow-black/40">
      {/* title bar */}
      <div className="flex items-center gap-2 border-b border-border bg-black/30 px-4 py-3">
        <span className="h-3 w-3 rounded-full bg-accent-red/70" />
        <span className="h-3 w-3 rounded-full bg-accent-yellow/70" />
        <span className="h-3 w-3 rounded-full bg-accent-green/70" />
        <span className="ml-3 text-xs text-muted">gradevault — 80x24</span>
      </div>

      {/* body */}
      <div className="overflow-x-auto px-5 py-5 font-mono text-[13px] leading-relaxed sm:text-sm">
        <div className="text-muted">$ ./gradevault</div>
        <Out>
          GradeVault - student grade management
          <br />
          Type &apos;help&apos; for commands. 0 student record(s) loaded from gradevault.dat.
        </Out>

        <Prompt>add &quot;Akshat&quot; 88 76 91 83</Prompt>
        <Out>Added Akshat - average 84.5, grade B [PASS]</Out>

        <Prompt>view all</Prompt>
        <Out>1 student(s) on record:</Out>
        <Out className="whitespace-pre text-foreground/80">
          Akshat          avg=84.5   grade=B  [PASS      ] marks: Math=88, Physics=76, Chemistry=91, English=83
        </Out>

        <Prompt>summary</Prompt>
        <div className="mt-1 space-y-0.5 rounded-lg border border-border/60 bg-black/20 px-3 py-2">
          <div className="text-accent-yellow">
            ⚠️ Class Average: 84.5 - Class is passing overall
          </div>
          <div>
            <span className="text-accent-red">🔴 0 students failing</span>
            <span className="text-muted"> | </span>
            <span className="text-accent-yellow">🟡 0 borderline</span>
            <span className="text-muted"> | </span>
            <span className="text-accent-green">🟢 1 passing</span>
          </div>
          <div className="text-accent-red/90">📉 Weakest subject: Physics (avg 76.0)</div>
          <div className="text-accent-green/90">📈 Strongest subject: Chemistry (avg 91.0)</div>
          <div className="text-muted">(No previous batch on record yet - this is the baseline.)</div>
          <div className="text-foreground">Solid batch - everyone is clearing the bar.</div>
        </div>

        <Prompt>export report.txt</Prompt>
        <Out>Exported report to report.txt</Out>

        <Prompt>exit</Prompt>
        <Out>Saved 1 student record(s) to gradevault.dat. Goodbye.</Out>

        <div className="mt-2 flex items-center gap-1 text-muted">
          <span className="text-accent-green">{">"}</span>
          <span className="h-4 w-2 animate-pulse bg-accent-green/70" />
        </div>
      </div>
    </div>
  );
}
