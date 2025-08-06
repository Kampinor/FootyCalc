def check_csv_format(csv_datei, expected_columns=7):
    """
    Prüft eine CSV-Datei auf Zeilen mit einer abweichenden Anzahl von Spalten.
    
    Args:
        csv_datei (str): Pfad zur CSV-Datei.
        expected_columns (int): Erwartete Anzahl an Spalten.
    """
    fehlerhafte_zeilen = []

    with open(csv_datei, 'r', encoding='utf-8', errors='ignore') as f:
        for i, line in enumerate(f, start=1):
            spalten = line.strip().split(',')
            if len(spalten) != expected_columns:
                fehlerhafte_zeilen.append((i, len(spalten), line.strip()))

    if fehlerhafte_zeilen:
        print(f"⚠️ Es wurden {len(fehlerhafte_zeilen)} fehlerhafte Zeilen gefunden:\n")
        for zeile in fehlerhafte_zeilen[:10]:  # nur die ersten 10 ausgeben
            print(f"  ➜ Zeile {zeile[0]}: {zeile[1]} Felder (Erwartet: {expected_columns})")
            print(f"    Inhalt: {zeile[2]}")
        if len(fehlerhafte_zeilen) > 10:
            print(f"... und {len(fehlerhafte_zeilen) - 10} weitere fehlerhafte Zeilen.")
    else:
        print("✅ Keine fehlerhaften Zeilen gefunden!")

# --- Testaufruf ---
if __name__ == "__main__":
    check_csv_format("DATA_2BL.csv", expected_columns=7)