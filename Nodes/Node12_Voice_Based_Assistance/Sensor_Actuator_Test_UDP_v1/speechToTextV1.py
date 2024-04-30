import socket
import wave
import speech_recognition as sr
import sys

# Server settings
server_ip = "0.0.0.0"  # Listen on all available interfaces
server_port = 9876

try:
    while True:
        try:
            # Create a UDP socket
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            server_socket.bind((server_ip, server_port))

            print("Server listening on {}:{}".format(server_ip, server_port))

            # Open the WAV file for writing (append mode)
            output_file = wave.open("received_audio.wav", "wb")
            output_file.setnchannels(1)  # Mono channel
            output_file.setsampwidth(2)  # 16-bit audio
            output_file.setframerate(44100)  # Sample rate 44.1 kHz

            stop_signal_received = False  # Flag to indicate if stop signal is received

            try:
                while not stop_signal_received:
                    data, addr = server_socket.recvfrom(1470)  # Receive audio data
                    if data == b"stop_sending":
                        stop_signal_received = True
                        print("Stop signal received. Stopping audio data reception.")
                    else:
                        output_file.writeframes(data)  # Write the received data to the WAV file
                        print("Received {} bytes from {}".format(len(data), addr))
            except KeyboardInterrupt:
                print("Server stopped by user")
                sys.exit(0)  # Exit the script gracefully
            except Exception as e:
                print("An error occurred:", e)
            finally:
                output_file.close()  # Close the WAV file
                server_socket.close()  # Close the socket

            if stop_signal_received:
                # Perform speech recognition
                recognizer = sr.Recognizer()
                with sr.AudioFile("received_audio.wav") as source:
                    audio_data = recognizer.record(source)
                    try:
                        recognized_text = recognizer.recognize_google(audio_data)
                        print("Recognized Text: {}".format(recognized_text))

                        # Send recognized text back to client using UDP
                        response_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                        response_socket.sendto(recognized_text.encode(), addr)  # Send back to the address that sent the audio

                    except sr.UnknownValueError:
                        print("Speech Recognition could not understand audio")
                    except sr.RequestError as e:
                        print("Could not request results from Google Speech Recognition service; {0}".format(e))
                        response_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                        response_socket.sendto("Recognition failed".encode(), addr)  # Send failure response
        except Exception as e:
            print("An error occurred:", e)
except KeyboardInterrupt:
    print("Server stopped by user")
