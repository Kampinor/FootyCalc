import pandas as pd

def analyse_fussball_daten(csv_datei='DATA_2BL (2).csv', anzahl_spiele=5):
    try:
        df = pd.read_csv(csv_datei)
    except FileNotFoundError:
        print(f"❌ Datei '{csv_datei}' nicht gefunden.")
        return None

    # ✅ Spalten korrekt umbenennen
    df.rename(columns={
        'Heimteam': 'Home',
        'Gastteam': 'Away',
        'Heimtore': 'HG',
        'Gastore': 'AG',
        'Heim XGoals': 'xHG',
        'Gast XGoals': 'xAG'
    }, inplace=True)

    # Zahlenfelder umwandeln
    for col in ['HG', 'AG', 'xHG', 'xAG']:
        df[col] = pd.to_numeric(df[col], errors='coerce')

    all_teams = pd.concat([df['Home'], df['Away']]).unique()
    results = []

    for team in all_teams:
        team_home = df[df['Home'] == team]
        team_away = df[df['Away'] == team]

        last_home = team_home.tail(anzahl_spiele)
        last_away = team_away.tail(anzahl_spiele)

        if last_home.empty and last_away.empty:
            continue

        avg_HG = last_home['HG'].mean() if not last_home.empty else 0
        avg_xHG = last_home['xHG'].mean() if not last_home.empty else 0
        avg_AG = last_away['AG'].mean() if not last_away.empty else 0
        avg_xAG = last_away['xAG'].mean() if not last_away.empty else 0

        avg_AG_gegen_home = last_home['AG'].mean() if not last_home.empty else 0
        avg_HG_gegen_away = last_away['HG'].mean() if not last_away.empty else 0

        angriffs_heim = round(avg_HG / avg_xHG, 4) if avg_xHG else 0
        angriffs_ausw = round(avg_AG / avg_xAG, 4) if avg_xAG else 0
        verteidigung_heim = round(avg_xHG / avg_AG_gegen_home, 4) if avg_AG_gegen_home else 0
        verteidigung_ausw = round(avg_xAG / avg_HG_gegen_away, 4) if avg_HG_gegen_away else 0

        results.append({
            'Team': team,
            'Angriffsstaerke_Heim': angriffs_heim,
            'Angriffsstaerke_Auswaerts': angriffs_ausw,
            'Verteidigungsstaerke_Heim': verteidigung_heim,
            'Verteidigungsstaerke_Auswaerts': verteidigung_ausw,
            'Heimspiele_gezählt': len(last_home),
            'Auswärtsspiele_gezählt': len(last_away)
        })

    result_df = pd.DataFrame(results)

    # ✅ Ergebnis speichern
    out_datei = csv_datei.replace('.csv', '_processed.csv')
    result_df.to_csv(out_datei, index=False)
    print(f"✅ Analyse abgeschlossen. Ergebnis gespeichert in {out_datei}")
    print(result_df.to_string(index=False))

    return result_df


# --- Skriptausführung ---
if __name__ == "__main__":
    analyse_fussball_daten('DATA_2BL (2).csv', anzahl_spiele=5)