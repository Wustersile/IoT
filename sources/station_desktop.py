from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtGui import *

import serial
import time
import threading
import sys


'''


'''


class Station:

    # Инициализация графических компонентов
    def __init__(self):
        self.app = QApplication(sys.argv)
        self.window = QMainWindow()
        self.pressure = QtWidgets.QLabel(self.window)
        self.humidity = QtWidgets.QLabel(self.window)
        self.temperatureOut = QtWidgets.QLabel(self.window)
        self.day = QtWidgets.QLabel(self.window)
        self.temperatureIn = QtWidgets.QLabel(self.window)
        self.time = QtWidgets.QLabel(self.window)
        self.date = QtWidgets.QLabel(self.window)

    def application(self):

        # Настройка Label
        self.window.setWindowTitle("Домашняя метеостанция")
        self.window.setGeometry(300, 400, 300, 250)
        self.window.setFixedSize(300, 250)

        # Настройка Label даты
        self.date.setFont(QFont('Arial', 12))
        self.date.move(210, 20)
        self.date.setFixedSize(300, 25)

        # Настройка Label времени
        self.time.setFont(QFont('Arial', 12))
        self.time.move(10, 20)
        self.time.setFixedSize(300, 25)

        # Настройка Label дня недели
        self.day.setFont(QFont('Arial', 12))
        self.day.move(255, 50)
        self.day.setFixedSize(300, 40)

        # Настройка Label температуры внутри
        self.temperatureIn.move(10, 90)
        self.temperatureIn.setFixedSize(300, 25)

        # Настройка Label температуры снаружи
        self.temperatureOut.move(10, 130)
        self.temperatureOut.setFixedSize(300, 25)

        # Настройка Label влажности
        self.humidity.move(10, 170)
        self.humidity.setFixedSize(300, 25)

        # Настройка Label давления
        self.pressure.move(10, 210)
        self.pressure.setFixedSize(300, 25)

        # работа с платой в отдельном потоке
        threading.Thread(target=self.start).start()

        self.window.show()
        sys.exit(self.app.exec_())

    def start(self):
        # Создание подключения
        arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)
        # Ожидание завершения подключения
        time.sleep(3)

        while True:
            # Считывание отправленной информации
            # Преобразование bytes в string
            line = arduino.readline().decode("utf-8")
            data = line.split('|')

            # Обновление времени
            if len(data) == 3:
                self.time.setText(data[0])
                self.date.setText(data[1])
                self.day.setText(data[2])

            # Обновление значений
            elif len(data) == 4:
                self.temperatureIn.setText("Температура внутри : " + data[0] + "C")
                self.temperatureOut.setText("Температура снаружи : " + data[1] + "C")
                self.humidity.setText("Влажность : " + data[2] + "%")
                self.pressure.setText("Давление : " + data[3] + "ммРтСт")


if __name__ == '__main__':
    # Создаение объекта метеостанции
    station = Station()
    # Запуск
    station.application()



