import matplotlib.pyplot as plt
import numpy as np
import keyboard
from math import *


def euler(u_0, precision, v_0):
    ValeurSuiteVitesse = [u_0]
    ValeurSuitePosition = [v_0]

    for i in range(precision - 1):
        ValeurSuiteVitesse.append(suiteVitesse(ValeurSuiteVitesse[i],
                                               precision))  # Calcule la prochaine valeur de u en utilisant la méthode de l'Euler
        ValeurSuitePosition.append(suitePosition(ValeurSuiteVitesse[i], precision, ValeurSuitePosition[i]))

    return ValeurSuitePosition, ValeurSuiteVitesse


def eulerExp2(u_0, precision, v_0):
    ValeurSuiteVitesse = [u_0]
    ValeurSuitePosition = [v_0]

    for i in range(precision - 1):
        ValeurSuiteVitesse.append(suiteVitesse2(ValeurSuiteVitesse[i], precision, ValeurSuitePosition[i],
                                                coeff_frottement))  # Calcule la prochaine valeur de u en utilisant la méthode de l'Euler
        ValeurSuitePosition.append(suitePosition2(ValeurSuiteVitesse[i], precision, ValeurSuitePosition[i]))

    return ValeurSuitePosition, ValeurSuiteVitesse


def suiteVitesse(u_n, precision):
    return u_n + (12.74 / precision) * (-5 * u_n - 9.81)


def suitePosition(u_n, precision, v_n):
    return v_n + (12.74 / precision) * u_n


def suiteVitesse2(u_n, precision, v_n, coeff_frottement):
    return u_n + (3 / precision) * (-coeff_frottement / masse * u_n - coeff_raideur / masse * v_n)


def suitePosition2(u_n, precision, v_n):
    return v_n + (3 / precision) * u_n


def vitesse(tabXVitesse):
    y = []  # Liste des valeurs de la vitesse

    for i in tabXVitesse:
        y.append(((9.81 * masse) / coeff_frottement) * (np.exp(-(coeff_frottement/masse) * i))-1)

    return y


def position(tabXVitesse):
    y2 = []

    for i in tabXVitesse:
        y2.append(-(((9.81 * masse) / coeff_frottement) * i) - (((masse / coeff_frottement) ** 2) * 9.81 * np.exp(
            -(coeff_frottement / masse) * i)) + ValeurInitPosition)

    return y2


def position2(tabXVitesse, coeff_qualite, coeff_frottement):
    y2 = []
    alpha = coeff_frottement / (2 * masse)
    omegaZeroCarre = coeff_raideur / masse
    if coeff_qualite > 1 / 2:
        for i in tabXVitesse:
            y2.append(np.exp(-alpha * i) * (ValeurInitPosition * np.cos(sqrt(omegaZeroCarre - (alpha) ** 2) * i) + (
                    (alpha * ValeurInitPosition + valeurInitVitesse) / sqrt(omegaZeroCarre - alpha ** 2)) * np.sin(
                sqrt(omegaZeroCarre - alpha ** 2) * i)))
    if coeff_qualite == 1 / 2:
        for i in tabXVitesse:
            y2.append(np.exp(-alpha * i) * ((valeurInitVitesse + alpha * ValeurInitPosition) * i + ValeurInitPosition))
    if coeff_qualite < 1 / 2:
        aide = sqrt(alpha ** 2 - omegaZeroCarre)
        for i in tabXVitesse:
            y2.append(np.exp(-alpha * i) * (
                    ((alpha * ValeurInitPosition - aide * ValeurInitPosition + valeurInitVitesse) / (-2 * aide))
                    * np.exp(-aide * i) + (ValeurInitPosition - (
                    (alpha * ValeurInitPosition - aide * ValeurInitPosition + valeurInitVitesse) / (
                    -2 * aide))) * np.exp(aide * i)))

    return y2


def activite1Vitesse(precision, choix):
    ValeurSuitePosition, ValeurSuiteVitesse = euler(valeurInitVitesse, precision, ValeurInitPosition)

    tabX = np.linspace(intervalMin, intervalMax, precision)
    tabXVitesse = np.linspace(intervalMin, intervalMax, 1000)
    fonctionVitesse = vitesse(tabXVitesse)
    fonctionPosition = position(tabXVitesse)

    for i in range(1000):
        fonctionVitesse[i] = abs(fonctionVitesse[i])

    for i in range(precision):
        ValeurSuiteVitesse[i] = abs(ValeurSuiteVitesse[i])

    if choix == 1:
        plt.plot(tabX, ValeurSuiteVitesse)
        plt.plot(tabXVitesse, fonctionVitesse)
        plt.xlabel('Temps (en s)')
        plt.ylabel('Vitesse (en m/s)')
        plt.title("Courbes de la vitesse de l'objet, en chute libre, en fonction du temps")
        plt.legend(["Courbe numérique de la vitesse", 'Courbe théorique de la vitesse'])
    if choix == 2:
        return calculMoyenneDiff(fonctionVitesse, ValeurSuiteVitesse, precision)
    if choix == 3:
        plt.plot(tabX, ValeurSuitePosition)
        plt.plot(tabXVitesse, fonctionPosition)
        plt.xlabel('Temps (en s)')
        plt.ylabel('Position (en m)')
        plt.title("Courbes de la position de l'objet, en chute libre, en fonction du temps")
        plt.legend(["Courbe numérique de la position", 'Courbe théorique de la position'])
    if choix == 4:
        return calculMoyenneDiff(fonctionPosition, ValeurSuitePosition, precision)


def activite2Vitesse(precision, choix):
    intervalMin = 0
    intervalMax = 3
    ValeurSuitePosition, ValeurSuiteVitesse = eulerExp2(valeurInitVitesse, precision, ValeurInitPosition)

    tabX = np.linspace(intervalMin, intervalMax, precision)
    tabXVitesse = np.linspace(intervalMin, intervalMax, 1000)
    fonctionPosition = position2(tabXVitesse, coeff_qualite, coeff_frottement)

    if choix == 3:
        plt.plot(tabX, ValeurSuitePosition)
        plt.plot(tabXVitesse, fonctionPosition)
        plt.xlabel('Temps (en s)')
        plt.ylabel('Position (en m/s)')
        plt.title("Courbes de la position de l'objet en fonction du temps.")
        plt.legend(['Courbe numérique de la position', 'Courbe théorique de la position'])
    if choix == 4:
        return calculMoyenneDiff(fonctionPosition, ValeurSuitePosition, precision)


def calculMoyenneDiff(fonction, suite, precision):
    somme = 0
    for i in range(precision):
        somme += abs(fonction[i] - suite[i])
    moyenne = somme / precision
    return moyenne


def activite1CourbeErreur():
    tabX = [50, 100, 250, 500, 750, 1000]
    tabY = []
    for i in tabX:
        if choix == 1:
            tabY.append(activite1Vitesse(i, 2))
        if choix == 2:
            tabY.append(activite1Vitesse(i, 4))
        if choix == 3:
            tabY.append(activite2Vitesse(i, 4))
    plt.plot(tabX, tabY)
    plt.xlabel('Précision')
    plt.ylabel("Moyenne d'erreur")
    plt.title("Courbe de l'erreur en fonction de la précision")


if __name__ == "__main__":
    choix = 0
    intervalMin = 0
    intervalMax = 13
    while choix != 4:
        print("Bienvenu(e) dans le menu! Que souhaitez-vous faire?\n1 - Afficher les courbes de vitesse l'activité 1\n"
              "2 - Afficher les courbes de la position de l'activité 1\n3 - Afficher les courbes de position de "
              "l'activité 2\n4 - Quitter\n")
        choix = int(input("Quel est votre choix ?\n"))
        if choix == 1:
            precision = int(input("Rentrez la précision voulue : "))
            ValeurInitPosition = float(input("Rentrez la valeur de la position initiale : "))
            valeurInitVitesse = float(input("Rentrez la valeur de la vitesse initiale : "))
            coeff_frottement = int(input("Rentrez la valeur du coefficient de frottement (lambda) : "))
            masse = int(input("Rentrez la masse de l'objet (m) : "))
            plt.clf()
            activite1Vitesse(precision, choix)
            plt.grid(True)
            plt.show()
            print("Appuyez sur la touche ECHAP pour continuer.")
            keyboard.wait('esc')
            plt.clf()
            activite1CourbeErreur()
        elif choix == 2:
            precision = int(input("Rentrez la précision voulue : "))
            ValeurInitPosition = float(input("Rentrez la valeur de la position initiale : "))
            valeurInitVitesse = float(input("Rentrez la valeur de la vitesse initiale : "))
            coeff_frottement = int(input("Rentrez la valeur du coefficient de frottement (lambda) : "))
            masse = int(input("Rentrez la masse de l'objet (m) : "))
            plt.clf()
            activite1Vitesse(precision, choix + 1)
            plt.grid(True)
            plt.show()
            print("Appuyez sur la touche ECHAP pour continuer.")
            keyboard.wait('esc')
            plt.clf()
            activite1CourbeErreur()
        elif choix == 3:
            precision = int(input("Rentrez la précision voulue : "))
            ValeurInitPosition = float(input("Rentrez la valeur de la position initiale : "))
            valeurInitVitesse = float(input("Rentrez la valeur de la vitesse initiale : "))
            coeff_frottement = int(input("Rentrez la valeur du coefficient de frottement (lambda) : "))
            coeff_qualite = (sqrt(1) * sqrt(100)) / coeff_frottement
            coeff_raideur = int(input("Rentrez le coefficient de raideur (k) : "))
            masse = int(input("Rentrez la masse de l'objet (m) : "))
            plt.clf()
            activite2Vitesse(precision, choix)
            plt.grid(True)
            plt.show()
            print("Appuyez sur la touche ECHAP pour continuer.")
            keyboard.wait('esc')
            plt.clf()
            activite1CourbeErreur()
        elif choix == 4:
            print("A bientôt, bonne journée!")
            break
        else:
            print('Cette valeur ne fait pas partie des choix proposés par le menu!')
        plt.grid(True)
        plt.show()