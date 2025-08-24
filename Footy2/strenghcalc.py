import pandas as pd

# CSV-Datei laden (Pfad anpassen)
df = pd.read_csv("Test.csv")

# 1. Neue Spalten HTCalc und GTCalc berechnen
df["HTCalc"] = (df["Heimtore"] + df["Heim XGoals"]) / 2
df["GTCalc"] = (df["Gastore"] + df["Gast XGoals"]) / 2

# 2. Ligaweite Durchschnittswerte
liga_durchschnitt = {
    "HTCalc_mean": df["HTCalc"].mean(),
    "GTCalc_mean": df["GTCalc"].mean()
}

# 3. Teamstatistiken berechnen
team_stats = {}
for team in pd.concat([df["Heimteam"], df["Gastteam"]]).unique():
    # Heimspiele des Teams
    heimspiele = df[df["Heimteam"] == team]
    # Auswärtsspiele des Teams
    auswaertsspiele = df[df["Gastteam"] == team]

    # Berechnungen
    heimtore = heimspiele["HTCalc"].mean() if not heimspiele.empty else 0
    heimgegentore = heimspiele["GTCalc"].mean() if not heimspiele.empty else 0
    auswaertstore = auswaertsspiele["GTCalc"].mean() if not auswaertsspiele.empty else 0
    auswaertsgegentore = auswaertsspiele["HTCalc"].mean() if not auswaertsspiele.empty else 0

    team_stats[team] = {
        "Heimtore_avg": heimtore,
        "HeimGegentore_avg": heimgegentore,
        "Auswaertstore_avg": auswaertstore,
        "AuswaertsGegentore_avg": auswaertsgegentore
    }


# Ergebnisse ausgeben, auf 2 Nachkommastellen gerundet
print("Ligaweite Durchschnittswerte:")
for k, v in liga_durchschnitt.items():
    print(f"{k}: {v:.2f}")

print("\nTeamdurchschnitte:")
for team, stats in team_stats.items():
    print(f"{team}:")
    for k, v in stats.items():
        print(f"  {k}: {v:.2f}")