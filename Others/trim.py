import os
import subprocess
import math
from pathlib import Path

def get_file_size_gb(filepath):
    """ファイル容量をGB単位で取得"""
    size_bytes = os.path.getsize(filepath)
    return size_bytes / (1024**3)

def get_video_duration(filepath):
    """動画の長さを秒単位で取得"""
    result = subprocess.run(
        ["ffprobe", "-v", "error", "-show_entries", "format=duration", "-of", "default=noprint_wrappers=1:nokey=1", filepath],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT
    )
    return float(result.stdout)

def split_video():
    # 入力ファイル名の指定
    file_name = input("処理するファイル名を入力してください (例: video.mp4): ").strip()
    
    if not os.path.exists(file_name):
        print(f"エラー: {file_name} が見つかりません。")
        return

    # 対応拡張子チェック
    ext = Path(file_name).suffix.lower()
    if ext not in ['.mp4', '.mov']:
        print("エラー: .mp4 または .mov ファイルを指定してください。")
        return

    # 容量表示
    size_gb = get_file_size_gb(file_name)
    print(f"ファイル容量: {size_gb:.2f} GB")

    # 保存先フォルダ作成
    output_dir = "done"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 動画の長さ取得と分割計算
    duration = get_video_duration(file_name)
    segment_duration = duration / 5
    base_name = Path(file_name).stem

    print(f"総再生時間: {duration:.2f}秒 (分割単位: {segment_duration:.2f}秒)")
    print("分割処理を開始します...")

    for i in range(5):
        start_time = i * segment_duration
        output_file = os.path.join(output_dir, f"{base_name}_{i+1}{ext}")
        
        # ffmpegコマンドの構築
        # -ss を入力の前に置くことで高速シーク（ストリーミング的な読み込み）を可能にし、
        # -c copy で再エンコードなし（高速・無劣化）で分割します。
        command = [
            "ffmpeg",
            "-y",                # 上書き許可
            "-ss", str(start_time),
            "-t", str(segment_duration),
            "-i", file_name,
            "-c", "copy",        # コーデックをコピー（高速）
            "-map", "0",
            "-avoid_negative_ts", "1", # 再生トラブル防止
            output_file
        ]

        subprocess.run(command, stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
        print(f"完了 ({i+1}/5): {output_file}")

    print("\nすべての処理が完了しました。'done' フォルダを確認してください。")

if __name__ == "__main__":
    split_video()