import os
import subprocess
import math
from pathlib import Path

def get_file_size_gb(filepath):
    size_bytes = os.path.getsize(filepath)
    return size_bytes / (1024**3)

def get_video_duration(filepath):
    result = subprocess.run(
        ["ffprobe", "-v", "error", "-show_entries", "format=duration", "-of", "default=noprint_wrappers=1:nokey=1", filepath],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT
    )
    return float(result.stdout)

def split_video():
    file_name = input("Input filename (e.g. video.mp4): ").strip()
    
    if not os.path.exists(file_name):
        print(f"Error: {file_name} not found.")
        return

    ext = Path(file_name).suffix.lower()
    if ext not in ['.mp4', '.mov']:
        print("Error: Invalid extension.")
        return

    size_gb = get_file_size_gb(file_name)
    print(f"File size: {size_gb:.2f} GB")

    output_dir = "done"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    duration = get_video_duration(file_name)
    segment_duration = duration / 5
    base_name = Path(file_name).stem

    print(f"Time: {duration:.2f}seconds (Time divided: {segment_duration:.2f}seconds)")
    print("Loading...")

    for i in range(5):
        start_time = i * segment_duration
        output_file = os.path.join(output_dir, f"{base_name}_{i+1}{ext}")

        command = [
            "ffmpeg",
            "-y",
            "-ss", str(start_time),
            "-t", str(segment_duration),
            "-i", file_name,
            "-c", "copy",
            "-map", "0",
            "-avoid_negative_ts", "1",
            output_file
        ]

        subprocess.run(command, stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
        print(f"完了 ({i+1}/5): {output_file}")

    print("\nDone. Check \"Done\" folder.")

if __name__ == "__main__":
    split_video()