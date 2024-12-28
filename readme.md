# Contrôleur Web de Matrice LED 788BS avec ESP32

Ce projet permet de contrôler une matrice LED 788BS 8x8 via une interface web hébergée sur un ESP32. Vous pouvez dessiner des motifs dans votre navigateur et les afficher instantanément sur la matrice LED.

## 📝 Table des matières
- [Matériel nécessaire](#matériel-nécessaire)
- [Schéma de branchement](#schéma-de-branchement)
- [Installation du logiciel](#installation-du-logiciel)
- [Configuration](#configuration)
- [Utilisation](#utilisation)
- [Dépannage](#dépannage)

## 🛠 Matériel nécessaire
- 1x ESP32 WROVER
- 1x Matrice LED 788BS (8x8)
- 8x Résistances 220Ω (pour les colonnes)
- Fils de connexion
- Breadboard
- Câble USB pour la programmation

## 📊 Schéma de branchement

### Schema de la Matrice LED 788BS

![Schéma de branchement](/images/788BSschematic.png)

### Exemple de branchement avec un arduino

![Exemple arduino](/images/ExempleSchema.png)

### Le rendu final
![Rendu final](/images/IMG_20241228_120802.jpg)

⚠️ **Important** : 
- Chaque broche de colonne (COL) doit être connectée via une résistance de 220Ω
- Les broches de ligne (ROW) sont connectées directement
- La matrice LED 788BS est en anode commune

## 💻 Installation du logiciel

### Prérequis
1. Arduino IDE installé sur votre ordinateur
2. Support ESP32 installé dans Arduino IDE
3. Bibliothèque ArduinoJson installée

### Installation de ArduinoJson
1. Ouvrez Arduino IDE
2. Allez dans Outils > Gérer les bibliothèques...
3. Recherchez "ArduinoJson"
4. Installez la dernière version

### Configuration de l'IDE
1. Sélectionnez la carte "ESP32 WROVER Module" dans Outils > Type de carte
2. Sélectionnez le bon port COM dans Outils > Port

## ⚙️ Configuration

1. Ouvrez le code dans Arduino IDE
2. Modifiez les identifiants WiFi :
```cpp
const char* ssid = "VotreSSID";
const char* password = "VotreMotDePasse";
```
3. Vérifiez que les broches GPIO correspondent à vos connexions

## 🚀 Utilisation

1. Téléversez le code sur l'ESP32
2. Ouvrez le moniteur série (115200 baud)
3. Notez l'adresse IP affichée
4. Ouvrez cette adresse dans votre navigateur web
5. Utilisez l'interface pour :
   - Cliquer sur les cases pour créer votre motif
   - Cliquer sur "Envoyer à la matrice" pour l'afficher
### Affichage web
![Affichage web](/images/webresult.png)

### Résultat
![Résultat](/images/ezgif-6-6d71266170.gif)
## 🔍 Dépannage

### La matrice ne s'allume pas
- Vérifiez les connexions des résistances
- Vérifiez la polarité de la matrice LED
- Testez chaque LED individuellement

### Impossible de se connecter au WiFi
- Vérifiez les identifiants WiFi
- Assurez-vous que l'ESP32 est dans la portée du réseau
- Vérifiez le moniteur série pour les messages d'erreur

### Motif incorrect
- Vérifiez l'ordre des connexions ROW et COL
- Assurez-vous que toutes les résistances sont bien connectées
- Vérifiez que la matrice n'est pas montée à l'envers


