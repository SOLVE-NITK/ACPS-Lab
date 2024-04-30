import socket
import wave

# Server settings
server_ip = "0.0.0.0"  # Listen on all available interfaces
server_port = 9876

# Create a UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind((server_ip, server_port))

print("Server listening on {}:{}".format(server_ip, server_port))

# Open the WAV file for writing (append mode)
output_file = wave.open("received_audio.wav", "wb")
output_file.setnchannels(1)  # Mono channel
output_file.setsampwidth(2)  # 16-bit audio
output_file.setframerate(44100)  # Sample rate 44.1 kHz

try:
    while True:
        data, addr = server_socket.recvfrom(1470)  # Receive audio data
        output_file.writeframes(data)  # Write the received data to the WAV file
        print("Received {} bytes from {}".format(len(data), addr))
except KeyboardInterrupt:
    print("Server stopped by user")

output_file.close()  # Close the WAV file
server_socket.close()  # Close the socket
