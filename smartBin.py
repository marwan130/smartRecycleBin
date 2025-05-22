import cv2
import tensorflow as tf
from tensorflow.keras.models import load_model
from tensorflow.keras.utils import img_to_array
import numpy as np
import serial
import time

model = load_model('trash_classification_model.keras')
classLabels = ["cardboard", "glass", "metal", "paper", "plastic", "trash"]

labelMapping = {
    "cardboard": "paper",
    "paper": "paper",
    "glass": "glass",
    "metal": "metal",
    "plastic": "trash",
    "trash": "trash"
}

# serial settings
arduinoPort = 'COM6'
baudRate = 9600
ser1 = serial.Serial('COM5', 9600)  

try:
    ser = serial.Serial(arduinoPort, baudRate, timeout=2)
    time.sleep(2)  
except serial.SerialException as e:
    print(f"Failed to connect to arduino: {e}")
    exit()

def classifyFrame(frame):
    img = cv2.resize(frame, (227, 227))
    img_array = img_to_array(img) / 255.0
    img_array = np.expand_dims(img_array, axis=0)
    pred = model.predict(img_array, verbose=0)
    original_label = classLabels[np.argmax(pred)]
    mapped_label = labelMapping.get(original_label)
    return mapped_label, np.max(pred)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # read from arduino
    if ser.in_waiting:
        line = ser.readline().decode().strip()
        print(f"Distance: {line}")
        
        if line == "within range":
            label, conf = classifyFrame(frame)
            print(f"Detected: {label} ({conf:.2f})")
            
            if label:
                print(f"Sent to Arduino: {label}")
                ser1.write(label.encode())
                

    cv2.imshow("Camera", frame)
    key = cv2.waitKey(1)
    if key == ord('q'):
        break

cap.release()
ser.close()
cv2.destroyAllWindows()
