import pandas as pd
import os

# Eingabedatei (Pfad anpassen)
input_file = "Test.csv"

# Ausgabe-Dateiname mit Suffix _processed
base, ext = os.path.splitext(input_file)
output_file = f"{base}_processed{ext}"

# CSV-Datei laden
df = pd.read_csv(input_file)

# Datum parsen & sortieren (damit 'letzte Spiele' korrekt sind)
#df["Datum"] = pd.to_datetime(df["Datum"])
#df = df.sort_values("Datum")

# Neue Spalten HTCalc und GTCalc berechnen
df["HTCalc"] = (df["Heimtore"] + df["Heim XGoals"]) / 2
df["GTCalc"] = (df["Gastore"] + df["Gast XGoals"]) / 2

# Ligaweite Durchschnittswerte
liga_durchschnitt = {
    "HTCalc_mean": df["HTCalc"].mean(),
    "GTCalc_mean": df["GTCalc"].mean()
}

# Parameter: maximale Anzahl an Spielen pro Team berücksichtigen
MAX_SPIELE = 5   # Beispiel: nur die letzten 5 Heim- und Auswärtsspiele

# 4. Teamstatistiken berechnen
team_rows = []
for team in pd.concat([df["Heimteam"], df["Gastteam"]]).unique():
    heimspiele = df[df["Heimteam"] == team].tail(MAX_SPIELE)
    auswaertsspiele = df[df["Gastteam"] == team].tail(MAX_SPIELE)

    heimtore = heimspiele["HTCalc"].mean() if not heimspiele.empty else 0
    heimgegentore = heimspiele["GTCalc"].mean() if not heimspiele.empty else 0
    auswaertstore = auswaertsspiele["GTCalc"].mean() if not auswaertsspiele.empty else 0
    auswaertsgegentore = auswaertsspiele["HTCalc"].mean() if not auswaertsspiele.empty else 0

    team_rows.append({
        "Team": team,
        "Heimtore_avg": heimtore,
        "HeimGegentore_avg": heimgegentore,
        "Auswaertstore_avg": auswaertstore,
        "AuswaertsGegentore_avg": auswaertsgegentore,
        "Heimtore_rel": heimtore / liga_durchschnitt["HTCalc_mean"] if liga_durchschnitt["HTCalc_mean"] else None,
        "HeimGegentore_rel": heimgegentore / liga_durchschnitt["GTCalc_mean"] if liga_durchschnitt["GTCalc_mean"] else None,
        "Auswaertstore_rel": auswaertstore / liga_durchschnitt["GTCalc_mean"] if liga_durchschnitt["GTCalc_mean"] else None,
        "AuswaertsGegentore_rel": auswaertsgegentore / liga_durchschnitt["HTCalc_mean"] if liga_durchschnitt["HTCalc_mean"] else None
    })

#  In DataFrame umwandeln
team_stats_df = pd.DataFrame(team_rows)

# Werte auf drei Nachkommastellen runden
team_stats_df = team_stats_df.round(3)

#  Ergebnisse speichern mit _processed-Suffix
team_stats_df.to_csv(output_file, index=False)

print(f"Teamstatistiken gespeichert in: {output_file}")