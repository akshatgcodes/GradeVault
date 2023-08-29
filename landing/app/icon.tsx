import { ImageResponse } from "next/og";

export const size = { width: 32, height: 32 };
export const contentType = "image/png";

export default function Icon() {
  return new ImageResponse(
    (
      <div
        style={{
          width: "100%",
          height: "100%",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          background: "#0a0f0c",
          borderRadius: 6,
          color: "#4ade80",
          fontFamily: "monospace",
          fontSize: 18,
          fontWeight: 700,
        }}
      >
        {">_"}
      </div>
    ),
    { ...size }
  );
}
