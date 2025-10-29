import pandas as pd
import numpy as np
from scipy.stats import poisson

# ----------------------------------------------------------
# 1️⃣ Spielpaarungen aus Datei (paarungen.csv) einlesen
# ----------------------------------------------------------
def lade_spiele(datei):
    with open(datei, "r", encoding="utf-8-sig") as f:
        lines = [line.strip() for line in f if line.strip()]

    spiele = []
    i = 0
    while i < len(lines):
        # Zeile mit "vs" finden
        if " vs " in lines[i]:
            datum = lines[i - 1] if i > 0 else ""
            begegnung = lines[i]
            try:
                heim_xg = float(lines[i + 1].replace(",", "."))
                auswaerts_xg = float(lines[i + 2].replace(",", "."))
                total_xg = float(lines[i + 3].replace(",", ".")) if i + 3 < len(lines) else None
            except ValueError:
                heim_xg, auswaerts_xg, total_xg = np.nan, np.nan, np.nan

            parts = begegnung.split(" vs ")
            heim = parts[0].strip()
            auswaerts = parts[1].strip()

            spiele.append({
                "Datum": datum,
                "Begegnung": begegnung,
                "Heim": heim,
                "Auswärts": auswaerts,
                "Heim_xG": heim_xg,
                "Auswärts_xG": auswaerts_xg,
                "Total_xG": total_xg
            })
            i += 4
        else:
            i += 1

    df = pd.DataFrame(spiele)
    print(f"\n✅ {len(df)} Begegnungen eingelesen.")
    return df


# ----------------------------------------------------------
# 2️⃣ Teamdaten einlesen (teamdaten.csv)
# ----------------------------------------------------------
def lade_teamdaten(datei):
    df = pd.read_csv(datei, encoding="utf-8-sig")
    df.columns = df.columns.str.strip()
    df = df.map(lambda x: x.strip() if isinstance(x, str) else x)
    print(f"✅ {len(df)} Teams eingelesen.")
    return df


# ----------------------------------------------------------
# 3️⃣ Quotenberechnung (Poisson)
# ----------------------------------------------------------
def berechne_torquoten(team_home, team_away, df, max_tore=10):
    try:
        home = df.loc[df["Team"] == team_home].iloc[0]
        away = df.loc[df["Team"] == team_away].iloc[0]
    except IndexError:
        print(f"⚠️ Teamdaten fehlen für '{team_home}' oder '{team_away}' – übersprungen.")
        return None

    # Erwartete Tore
    lambda_home = home["Heimtore_rel"] * away["AuswaertsGegentore_rel"]
    lambda_away = away["Auswaertstore_rel"] * home["HeimGegentore_rel"]

    # Poisson
    p_home_goals = [poisson.pmf(i, lambda_home) for i in range(max_tore + 1)]
    p_away_goals = [poisson.pmf(i, lambda_away) for i in range(max_tore + 1)]
    matrix = np.outer(p_home_goals, p_away_goals)

    # Ergebnisse
    p_home_win = np.tril(matrix, -1).sum()
    p_draw = np.trace(matrix)
    p_away_win = np.triu(matrix, 1).sum()

    total_goals = np.add.outer(range(max_tore + 1), range(max_tore + 1))
    p_over25 = matrix[total_goals > 2].sum()
    p_under25 = matrix[total_goals <= 2].sum()

    # Quote = 1 / Wahrscheinlichkeit
    to_q = lambda p: round(1 / p, 2) if p > 0 else np.inf
    return {
        "Home": to_q(p_home_win),
        "Draw": to_q(p_draw),
        "Away": to_q(p_away_win),
        "Over2.5": to_q(p_over25),
        "Under2.5": to_q(p_under25)
    }


# ----------------------------------------------------------
# 4️⃣ Hauptteil: alles kombinieren
# ----------------------------------------------------------
if __name__ == "__main__":
    # Dateien einlesen
    df_teams = lade_teamdaten("DATA_2BL_processed.csv")
    df_spiele = lade_spiele("paarungen.csv")

    ergebnisse = []

    for _, row in df_spiele.iterrows():
        team_home = row["Heim"]
        team_away = row["Auswärts"]
        quoten = berechne_torquoten(team_home, team_away, df_teams)
        if quoten:
            ergebnisse.append({
                "Datum": row["Datum"],
                "Begegnung": row["Begegnung"],
                **quoten
            })

    df_quoten = pd.DataFrame(ergebnisse)
    print("\n📊 Berechnete Quoten:")
    print(df_quoten.head())

    # In Datei speichern
    df_quoten.to_csv("berechnete_quoten.csv", index=False, encoding="utf-8-sig")
    print("\n💾 Ergebnisse gespeichert in: berechnete_quoten.csv")