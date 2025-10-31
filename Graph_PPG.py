import matplotlib.pyplot as plt

# Nom du fichier contenant les données
fichier = "log.txt"

# Listes pour stocker les valeurs des deux colonnes
col1 = []
col2 = []

# Lecture du fichier
with open(fichier, "r") as f:
    for ligne in f:
        if ligne.strip() == "":  # ignore les lignes vides
            continue
        valeurs = ligne.split()
        if len(valeurs) >= 2:  # s'assure qu'il y a au moins deux colonnes
            col1.append(float(valeurs[0]))
            col2.append(float(valeurs[1]))

# --- Premier graphique ---
plt.figure(1)
plt.plot(col1, label="Première colonne", marker='o', color='b')
plt.title("Tracé de la première colonne du fichier")
plt.xlabel("Index (numéro de ligne)")
plt.ylabel("Valeur colonne 1")
plt.legend()
plt.grid(True)

# --- Deuxième graphique ---
plt.figure(2)
plt.plot(col2, label="Deuxième colonne", marker='s', color='r')
plt.title("Tracé de la deuxième colonne du fichier")
plt.xlabel("Index (numéro de ligne)")
plt.ylabel("Valeur colonne 2")
plt.legend()
plt.grid(True)

# Affichage des deux graphiques dans deux fenêtres distinctes
plt.show()
