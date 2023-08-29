export default function Badge({ children }: { children: React.ReactNode }) {
  return (
    <span className="inline-flex items-center rounded-full border border-border bg-background-elevated px-3.5 py-1.5 font-mono text-xs text-foreground/90">
      {children}
    </span>
  );
}
