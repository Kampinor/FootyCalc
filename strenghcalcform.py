import pandas as pd
import os

# Eingabedatei
input_file = "DATA_2BL.csv"

# Ausgabe-Dateiname mit _processed
base, ext = os.path.splitext(input_file)
output_file = f"{base}_processed{ext}"

# CSV laden
df = pd.read_csv(input_file)

# Datum nur als String sortieren (dd/mm funktioniert trotzdem)
df = df.sort_values("Datum")

# Neue Spalten berechnen
df["HTCalc"] = (df["Heimtore"] + df["Heim XGoals"]) / 2
df["GTCalc"] = (df["Gastore"] + df["Gast XGoals"]) / 2

# Liga-Durchschnitt
liga_durchschnitt = {
    "HTCalc_mean": df["HTCalc"].mean(),
    "GTCalc_mean": df["GTCalc"].mean()
}

# Parameter
MAX_SPIELE = 5
FORM_SPIELE = 3


# Punkteberechnung
def berechne_punkte(row, team):
    if row["Heimteam"] == team:
        if row["Heimtore"] > row["Gastore"]:
            return 3
        elif row["Heimtore"] == row["Gastore"]:
            return 1
        else:
            return 0
    else:
        if row["Gastore"] > row["Heimtore"]:
            return 3
        elif row["Gastore"] == row["Heimtore"]:
            return 1
        else:
            return 0


# Form-Mapping
def form_mapping(punkte):

    if 0 <= punkte <= 1:
        return 0.9
    elif 2 <= punkte <= 3:
        return 0.95
    elif 4 <= punkte <= 5:
        return 1.0
    elif 6 <= punkte <= 7:
        return 1.05
    elif 8 <= punkte <= 9:
        return 1.1
    else:
        return 1.0


team_rows = []

teams = pd.concat([df["Heimteam"], df["Gastteam"]]).unique()

for team in teams:

    heimspiele = df[df["Heimteam"] == team].tail(MAX_SPIELE)
    auswaertsspiele = df[df["Gastteam"] == team].tail(MAX_SPIELE)

    heim_count = len(heimspiele)
    auswaerts_count = len(auswaertsspiele)
    gesamt_count = heim_count + auswaerts_count

    heimtore = heimspiele["HTCalc"].mean() if heim_count > 0 else 0
    heimgegentore = heimspiele["GTCalc"].mean() if heim_count > 0 else 0

    auswaertstore = auswaertsspiele["GTCalc"].mean() if auswaerts_count > 0 else 0
    auswaertsgegentore = auswaertsspiele["HTCalc"].mean() if auswaerts_count > 0 else 0

    # Formberechnung (letzte Spiele insgesamt)
    team_spiele = df[
        (df["Heimteam"] == team) | (df["Gastteam"] == team)
    ].tail(FORM_SPIELE)

    punkte = sum(
        berechne_punkte(row, team)
        for _, row in team_spiele.iterrows()
    )

    formstaerke = form_mapping(punkte)

    team_rows.append({

        "Team": team,

        "Heimspiele_count": heim_count,
        "Auswaertsspiele_count": auswaerts_count,
        "Spiele_count": gesamt_count,

        "Heimtore_avg": heimtore,
        "HeimGegentore_avg": heimgegentore,
        "Auswaertstore_avg": auswaertstore,
        "AuswaertsGegentore_avg": auswaertsgegentore,

        "Heimtore_rel": heimtore / liga_durchschnitt["HTCalc_mean"] if liga_durchschnitt["HTCalc_mean"] else None,
        "HeimGegentore_rel": heimgegentore / liga_durchschnitt["GTCalc_mean"] if liga_durchschnitt["GTCalc_mean"] else None,

        "Auswaertstore_rel": auswaertstore / liga_durchschnitt["GTCalc_mean"] if liga_durchschnitt["GTCalc_mean"] else None,
        "AuswaertsGegentore_rel": auswaertsgegentore / liga_durchschnitt["HTCalc_mean"] if liga_durchschnitt["HTCalc_mean"] else None,

        "Formstaerke": formstaerke
    })


# DataFrame erstellen
team_stats_df = pd.DataFrame(team_rows)

# Rundung
numeric_cols = team_stats_df.select_dtypes(include="number").columns

for col in numeric_cols:
    if "count" not in col.lower():
        team_stats_df[col] = team_stats_df[col].round(3)


# CSV speichern
team_stats_df.to_csv(output_file, index=False)

print("Teamstatistiken gespeichert in:", output_file)