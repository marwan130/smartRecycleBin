import cv2
import tensorflow as tf
from tensorflow.keras.models import load_model
from tensorflow.keras.utils import img_to_array
import numpy as np
import serial

model = load_model('trash_classification_model.keras')
class_labels = ["cardboard", "glass", "metal", "paper", "plastic", "trash"]

# connect to microcontroller over serial
ser = serial.Serial('COM3', 9600)  

def classify_frame(frame):
    img = cv2.resize(frame, (227, 227))
    img_array = img_to_array(img) / 255.0
    img_array = np.expand_dims(img_array, axis=0)
    pred = model.predict(img_array)
    return class_labels[np.argmax(pred)], np.max(pred)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    cv2.imshow("Camera", frame)

    key = cv2.waitKey(1)
    if key == ord('c'):  # press 'c' to classify
        label, conf = classify_frame(frame)
        print(f"Detected: {label} ({conf:.2f})")

        # send command to microcontroller
        ser.write(label.encode())  # microcontroller will handle what to do based on label

    elif key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
