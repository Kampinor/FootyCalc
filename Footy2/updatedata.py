import csv
import os

def lese_csv(datei):
    """Liest eine CSV-Datei mit Fußballergebnissen und speichert sie in einer Map"""
    ergebnisse = {}  
    if not os.path.exists(datei):
        return ergebnisse  # Falls Datei nicht existiert, leere Map zurückgeben
    
    with open(datei, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) < 6:
                continue
            heim, auswärts, tore_heim, tore_auswärts, xg_heim, xg_auswärts = row[:6]
            ergebnisse[(heim, auswärts)] = (tore_heim, tore_auswärts, xg_heim, xg_auswärts)
    return ergebnisse

def lese_textdatei(datei):
    """Liest eine Textdatei ein und speichert jede Zeile als Element einer Liste"""
    with open(datei, mode='r', encoding='utf-8') as file:
        return [line.strip() for line in file.readlines()]

def extrahiere_tore(zeile):
    """Bereinigt das Torergebnis ('1 - 0VZ') und gibt Tore als Tupel zurück"""
    zeile = zeile.replace("VZ", "").strip()  # 'VZ' entfernen und Leerzeichen trimmen
    teile = zeile.split(" - ")  # String an " - " aufteilen
    if len(teile) == 2 and teile[0].isdigit() and teile[1].isdigit():
        return teile[0], teile[1]
    return "", ""  # Falls Format unerwartet ist, leere Werte zurückgeben

def aktualisiere_csv_und_map(csv_datei, text_datei):
    ergebnisse = lese_csv(csv_datei)
    zeilen = lese_textdatei(text_datei)
    
    neue_einträge = []

    for i in range(len(zeilen)):
        if 'Quoten' in zeilen[i]:
            if i > 0 and i + 11 < len(zeilen):  # Sicherstellen, dass Indizes gültig sind
                heim = zeilen[i - 1]
                auswärts = zeilen[i + 11]
                
                xg_heim = zeilen[i + 2] if i + 3 < len(zeilen) else ''
                xg_auswärts = zeilen[i + 8] if i + 9 < len(zeilen) else ''

                tore_zeile = zeilen[i + 5] if i + 5 < len(zeilen) else ''
                tore_heim, tore_auswärts = extrahiere_tore(tore_zeile)

                if (heim, auswärts) not in ergebnisse:
                    ergebnisse[(heim, auswärts)] = (tore_heim, tore_auswärts, xg_heim, xg_auswärts)
                    neue_einträge.append([heim, auswärts, tore_heim, tore_auswärts, xg_heim, xg_auswärts])

    # Neue Einträge in die CSV-Datei schreiben
    if neue_einträge:
        with open(csv_datei, mode='a', encoding='utf-8', newline='') as file:
            writer = csv.writer(file)
            writer.writerows(neue_einträge)

# Beispielaufruf:
aktualisiere_csv_und_map("DATA_2BL.csv", "testneu.txt")