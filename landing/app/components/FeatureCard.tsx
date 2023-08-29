export default function FeatureCard({
  title,
  children,
}: {
  title: string;
  children: React.ReactNode;
}) {
  return (
    <div className="rounded-lg border border-border bg-background-elevated p-4">
      <div className="mb-1.5 font-mono text-sm text-accent-green">{title}</div>
      <p className="text-sm leading-relaxed text-muted">{children}</p>
    </div>
  );
}
