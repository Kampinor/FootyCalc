import pandas as pd

def process_football_data(input_csv_path, output_csv_path=None, target_season=None):
    """
    Verarbeitet eine CSV-Datei mit Fußballspielergebnissen:
    - Wählt spezifische Spalten aus.
    - Berechnet die Punkte für Heim- und Auswärtsteams.
    - Filtert optional nach einer bestimmten Saison.

    Args:
        input_csv_path (str): Pfad zur Eingabe-CSV-Datei.
        output_csv_path (str, optional): Pfad zum Speichern der Ausgabe-CSV-Datei.
                                        Wenn None, wird die DataFrame zurückgegeben.
        target_season (str, optional): Die Saison, nach der gefiltert werden soll (z.B. '2024/2025').
                                       Wenn None, werden alle Saisons verarbeitet.

    Returns:
        pandas.DataFrame or None: Die verarbeitete DataFrame, wenn output_csv_path None ist,
                                  sonst None.
    """
    try:
        df = pd.read_csv(input_csv_path)
    except FileNotFoundError:
        print(f"Fehler: Die Datei '{input_csv_path}' wurde nicht gefunden.")
        return None

    # Optional: Filtern nach einer bestimmten Saison
    if target_season:
        # Stellen Sie sicher, dass die 'Season'-Spalte den richtigen Datentyp hat (z.B. String)
        df['Season'] = df['Season'].astype(str)
        df = df[df['Season'] == target_season].copy()
        if df.empty:
            print(f"Warnung: Keine Daten für die Saison '{target_season}' gefunden. Leere DataFrame wird zurückgegeben.")
            return None # Oder eine leere DataFrame zurückgeben, je nach gewünschtem Verhalten

    # Schritt 1: Erforderliche Spalten auswählen
    # Überprüfen Sie, ob die benötigten Spalten existieren, bevor Sie sie auswählen
    required_columns = ['Season', 'Date', 'Home', 'Away', 'HG', 'AG']
    missing_columns = [col for col in required_columns if col not in df.columns]
    if missing_columns:
        print(f"Fehler: Folgende benötigte Spalten fehlen in der Datei: {', '.join(missing_columns)}")
        return None

    df_filtered = df[required_columns].copy()

    # Schritt 2: Punkteberechnung
    # Punkte für das Heimteam
    df_filtered['HomePoints'] = 0
    df_filtered.loc[df_filtered['HG'] > df_filtered['AG'], 'HomePoints'] = 3  # Heimsieg
    df_filtered.loc[df_filtered['HG'] == df_filtered['AG'], 'HomePoints'] = 1  # Unentschieden

    # Punkte für das Auswärtsteam
    df_filtered['AwayPoints'] = 0
    df_filtered.loc[df_filtered['AG'] > df_filtered['HG'], 'AwayPoints'] = 3  # Auswärtssieg
    df_filtered.loc[df_filtered['HG'] == df_filtered['AG'], 'AwayPoints'] = 1  # Unentschieden

    # Optional: Speichern der modifizierten Daten
    if output_csv_path:
        df_filtered.to_csv(output_csv_path, index=False)
        print(f"Verarbeitete Daten wurden erfolgreich in '{output_csv_path}' gespeichert.")
    else:
        return df_filtered

# Beispielaufruf, um nur die Daten der Saison 2024/2025 zu verarbeiten:
input_file = 'MEX.csv'
output_file = 'processed_MEX_2024_2025.csv'
target_season_to_process = '2024/2025' # Die gewünschte Saison

process_football_data(input_file, output_file, target_season=target_season_to_process)

# Wenn Sie die DataFrame direkt sehen möchten:
df_processed_2024_2025 = process_football_data(input_file, target_season=target_season_to_process)
if df_processed_2024_2025 is not None:
    print(df_processed_2024_2025.head())
    print(f"Anzahl der Zeilen für Saison {target_season_to_process}: {len(df_processed_2024_2025)}")
