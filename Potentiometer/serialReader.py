import serial
import csv

# Open a serial connection to the Arduino
ser = serial.Serial('COM5', 9600)  # Change 'COM3' to match your Arduino's serial port

# Open a CSV file for writing
with open('data.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)

    # Write a header row if needed
    csv_writer.writerow(['Room', 'Potent Value'])  # Add column headers as needed

    while True:
        data = ser.readline().decode().strip()
        if data:  # Check if data is not empty
            data_list = data.split(',')  # Assuming data is comma-separated
            csv_writer.writerow(data_list)
            print(data)  # Optional: print the data to the console

file.close()