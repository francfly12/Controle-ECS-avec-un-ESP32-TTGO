# Controle-ECS-avec-un-ESP32-TTGO
Controle de la puissance de chauffe d'un chauffe eau electrique - utilisation d'un module ESP32 TTGO - commentaires en Français
![20210829_223855](https://user-images.githubusercontent.com/16886296/131267693-ceaec2de-1e1c-413b-b61f-e74991a69c4a.jpg)

English users can ask for translation if needed.

Le systeme utilise une carte a **ESP32 lilygo.**
Le but est de piloter un chauffe eau électrique de façon a réguler sa consommation entre 0 et 100%. Ceci afin d'utiliser au mieux un système de génération électrique photovoltaique. Je crée ce Github car certains voudront peut etre utiliser cette carte pour d'autres circuits.
Je ne décris ici que la partie pilotage du chauffe eau. Il y a par ailleurs un circuit de mesure de puissance de l'installation qui donne des ordres à ce module ci.
![20210829_224256](https://user-images.githubusercontent.com/16886296/131266591-68f8126a-7ded-4c9b-9175-63dceb8d9774.jpg)

Le module lilygo ESP32 est une petite carte chinoise bon marché. Elles est peu documentée et de ce fait difficile à utiliser.
C'est de toute façon une copie de qualité moyenne. On peut accepter ou pas de bosser avec ce genre de matériel, sachant que la galère est de trouver les infos.
Elle comprend un ESP32 WROOM WIFI. 4 Mo de mémoire.
Attention car elle s'alimente sous 5V mais fonctionne en 3,3V.
Elle a un affichage OLED intégré.1,2 pouces.
Support de batterie 18650. Je l'ai déssoudé car inutile dans mon cas.

**Programmation IDE ARDUINO**
Pour la programmer, dans l'inteface Arduino, il faut d'abord charger les cartes ESP32 par les méthodes habituelles.
Ensuite choisir la carte "LOLIN D32", c'est ce qui marche le mieux.
Suivant les cas, le programme se lance automatiquement apres le chargement ou il fait appuyer manuellement sur le bouton EN pour resetter.
Si problème, il existe une possibilité pour mettre un capa sur la pin reset documentée sur le Net.  Dans mon cas, pas nécessaire.

**Alim**- Une Led verte montre l'alimentation de la carte. On peut alimenter depuis l'USB, ou, dans mon cas, directement par un broche Vin en 5V.
- une Led bleue supplémentaire est sur la pin 16. Elle sert dans ce cas a voir si on recoit des données sur le port série.
- il y a un circuit de charge et de protection pour une batterie 18650 qui doit donner plusieurs heures d'autonomie. 

**Affichage**
l'OLED est piloté par un SSD1306 en bus I2C. connecté sur les pins 5 (SDA) et 4 (SCL). le circuit est à l'adresse 0x3c.
la librairie U8G2lib marche très bien. Voir la doc sur github, il y a pléthore de fontes et de fonctions. Et avec cette librairie, pas de soucis d'interférences avec d'autres libraries.

**Bus Serie**
Le bus série  0 est mappé sur les pins D0 et D1, et passent par USB vers la console de l'IDE Arduino.(classique)
Pour avoir un deuxième port série, il faut utiliser un port virtuel et le mapper, ici sur les pins  16 et 17. 
D'ou utilisation d'une librairie HardwareSerial.h .J'ai réduit la tension à l'entrée de Rx par un pont de résistances à 2/3: pour passer d'un signal 5V à un 3,3V.

**Temperature** 
Le capteur est un classique ds18b20. Son utilisation est bien documentée; librarie ONE WIRE pour la connexion. Ne pas oublier une résistance de 2,7 K "tirant" la pin Data au +3,3V.

**Pilotage-Triac**
J'ai utilisé un dimmer module tout fait, de chez robotDYn. Connecté sur pin 25 (PSM, modulation ) et pin 26(signal zero crossing).
Librairie RBDdimmer.h.
![20210825_160534](https://user-images.githubusercontent.com/16886296/131267850-9fc950e2-f41c-4560-a304-7423ebd713dc.jpg)

Le **programme** est assez simple et documenté.
Il s'agit simplement de recevoir un ordre sur le bus série, qui correspond à une modulation de la chauffe entre 0 et 100%. L'ordre est exécuté par le module à triac.
En retour, on mesure la température de l'ECS. On affiche et on envoie la valeur sur le bus série.
En plus de cela, il existe deux modes: connecté (fonctionnement normal) mais aussi déconnecté (standalone, si la communication est perdue)
Il y a également une fonction anti-légionellose. Elle vérifie que la température n'est pas restée sous 45° pendant plus de 3 jours, et commande un cycle de chauffe à 60 degrés si necessaire.

**Boitier 3D**
Je peux fournir les STL des boitiers.




