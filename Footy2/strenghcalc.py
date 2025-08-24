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
    heimspiele = df[df["Heimteam"] == team]
    auswaertsspiele = df[df["Gastteam"] == team]

    heimtore = heimspiele["HTCalc"].mean() if not heimspiele.empty else 0
    heimgegentore = heimspiele["GTCalc"].mean() if not heimspiele.empty else 0
    auswaertstore = auswaertsspiele["GTCalc"].mean() if not auswaertsspiele.empty else 0
    auswaertsgegentore = auswaertsspiele["HTCalc"].mean() if not auswaertsspiele.empty else 0

    # Division durch Liga-Durchschnittswerte
    team_stats[team] = {
        "Heimtore_avg": heimtore,
        "HeimGegentore_avg": heimgegentore,
        "Auswaertstore_avg": auswaertstore,
        "AuswaertsGegentore_avg": auswaertsgegentore,
        "Heimtore_rel": heimtore / liga_durchschnitt["HTCalc_mean"] if liga_durchschnitt["HTCalc_mean"] else None,
        "HeimGegentore_rel": heimgegentore / liga_durchschnitt["GTCalc_mean"] if liga_durchschnitt["GTCalc_mean"] else None,
        "Auswaertstore_rel": auswaertstore / liga_durchschnitt["GTCalc_mean"] if liga_durchschnitt["GTCalc_mean"] else None,
        "AuswaertsGegentore_rel": auswaertsgegentore / liga_durchschnitt["HTCalc_mean"] if liga_durchschnitt["HTCalc_mean"] else None
    }

# Ergebnisse ausgeben
print("Ligaweite Durchschnittswerte:")
for k, v in liga_durchschnitt.items():
    print(f"{k}: {v:.2f}")

print("\nTeamdurchschnitte (absolut und relativ):")
for team, stats in team_stats.items():
    print(f"\n{team}:")
    for k, v in stats.items():
        print(f"  {k}: {v:.2f}")