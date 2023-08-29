export default function CodeBlock({ lines }: { lines: string[] }) {
  return (
    <pre className="overflow-x-auto rounded-lg border border-border bg-background-elevated px-4 py-3 font-mono text-[13px] leading-relaxed text-foreground/90 sm:text-sm">
      {lines.map((line, i) => (
        <div key={i}>
          {line.startsWith("#") ? (
            <span className="text-muted">{line}</span>
          ) : (
            <>
              <span className="text-accent-green">$ </span>
              {line}
            </>
          )}
        </div>
      ))}
    </pre>
  );
}
