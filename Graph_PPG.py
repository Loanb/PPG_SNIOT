import matplotlib.pyplot as plt

# Nom du fichier contenant les données
fichier = "log.txt"

# Liste pour stocker les valeurs de la première colonne
col1 = []

# Lecture du fichier
with open(fichier, "r") as f:
    for ligne in f:
        if ligne.strip() == "":  # ignore les lignes vides
            continue
        valeurs = ligne.split()
        col1.append(float(valeurs[0]))  # prend uniquement la première colonne

# Tracé du graphique
plt.figure()
plt.plot(col1, label="Première colonne", marker='o')

# Personnalisation
plt.title("Tracé de la première colonne du fichier")
plt.xlabel("Index (numéro de ligne)")
plt.ylabel("Valeur")
plt.legend()
plt.grid(True)

# Affichage du graphique
plt.show()
