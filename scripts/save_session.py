#!/usr/bin/env python3
"""Reads a Claude Code Stop hook payload from stdin and writes the session to markdown."""

import json
import os
import sys
from datetime import datetime
from pathlib import Path


def get_project_dir(cwd: str) -> Path:
    project_hash = cwd.replace("/", "-")
    return Path.home() / ".claude" / "projects" / project_hash


def parse_session(jsonl_path: Path) -> list[dict]:
    messages = []
    with open(jsonl_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                obj = json.loads(line)
            except json.JSONDecodeError:
                continue

            if obj.get("type") not in ("user", "assistant"):
                continue

            content = obj.get("message", {}).get("content", [])
            if isinstance(content, str):
                text = content
            else:
                text = "\n\n".join(
                    item["text"]
                    for item in content
                    if item.get("type") == "text" and item.get("text", "").strip()
                )

            if not text.strip():
                continue

            messages.append({
                "role": obj["type"],
                "text": text,
                "timestamp": obj.get("timestamp", ""),
            })

    return messages


def render_markdown(messages: list[dict], session_id: str, date_str: str) -> str:
    lines = [
        f"# Session — {date_str}",
        "",
        f"**Session ID:** `{session_id}`",
        "",
        "---",
        "",
    ]

    for msg in messages:
        heading = "## You" if msg["role"] == "user" else "## Claude"
        lines += [heading, "", msg["text"], "", "---", ""]

    return "\n".join(lines)


def main():
    raw = sys.stdin.read().strip()
    if not raw:
        sys.exit(0)

    try:
        hook_data = json.loads(raw)
    except json.JSONDecodeError:
        sys.exit(0)

    session_id = hook_data.get("session_id")
    cwd = hook_data.get("cwd", os.getcwd())

    if not session_id:
        sys.exit(0)

    jsonl_path = get_project_dir(cwd) / f"{session_id}.jsonl"
    if not jsonl_path.exists():
        sys.exit(0)

    messages = parse_session(jsonl_path)
    if not messages:
        sys.exit(0)

    # Derive date/time from first message timestamp for stable filenames.
    try:
        dt = datetime.fromisoformat(messages[0]["timestamp"].replace("Z", "+00:00"))
        date_str = dt.strftime("%Y-%m-%d")
        time_str = dt.strftime("%H%M%S")
    except (ValueError, AttributeError, KeyError):
        now = datetime.now()
        date_str = now.strftime("%Y-%m-%d")
        time_str = now.strftime("%H%M%S")

    output_dir = Path(cwd) / "sessions"
    output_dir.mkdir(exist_ok=True)

    # Filename is stable per session so each Stop-hook fire overwrites the same file,
    # leaving the complete conversation at the end of the session.
    filename = f"{date_str}-{time_str}-{session_id[:8]}.md"
    output_path = output_dir / filename

    content = render_markdown(messages, session_id, f"{date_str} at {time_str[:2]}:{time_str[2:4]}")
    output_path.write_text(content, encoding="utf-8")


if __name__ == "__main__":
    main()
