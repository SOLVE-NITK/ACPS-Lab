from flask import Flask, request
import socket
import speech_recognition as sr

app = Flask(__name__)

UDP_IP = "0.0.0.0"  # Listen on all available interfaces
UDP_PORT = 9876  # Change UDP port

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

recognizer = sr.Recognizer()

@app.route('/speechToText', methods=['POST'])
def process_audio():
    audio_data = request.data
    
    # Perform speech recognition on audio_data
    recognized_text = recognize_audio(audio_data)
    
    return recognized_text

def recognize_audio(audio_data):
    try:
        with sr.AudioFile(audio_data) as source:
            audio = recognizer.record(source)  # Load the audio file
            recognized_text = recognizer.recognize_google(audio)  # Use Google Web Speech API
            return recognized_text
    except sr.UnknownValueError:
        return "Speech not recognized"
    except sr.RequestError as e:
        return f"Error with the recognition service; {e}"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=9876)  # Change app port here
