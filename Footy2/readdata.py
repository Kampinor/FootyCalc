import pandas as pd

# CSV-Datei laden
df = pd.read_csv('2_bundesliga_data_neu.csv',header=None)

# Erste Zeilen anzeigen
#print(df.head()) # 1. 5 zeilen
#print(df.info()) # spalteninfo

# Spalten manuell benennen
df.columns = ['Wochentag','Datum','Uhrzeit','HTeam','Heim_xG','Heim_Tore', 'Gast_Tore','Gast_xG','GTeam','Zuschauer','Stadion','Schiri','Spielbericht','Verweise']

#df['Punkte_Heim'] = df.apply(lambda x: 3 if x['Heim_Tore'] > x['Gast_Tore'] else (1 if x['Heim_Tore'] == x['Gast_Tore'] else 0), axis=1)
#df['Punkte_Gast'] = df.apply(lambda x: 3 if x['Gast_Tore'] > x['Heim_Tore'] else (1 if x['Heim_Tore'] == x['Gast_Tore'] else 0), axis=1)

#print(df.head()) 


mannschaft = "PAD"
heimspiele = df[df['HTeam'] == mannschaft]

# Mehrere Spalten entfernen
heimspiele = heimspiele.drop(columns=['Wochentag', 'Datum','Uhrzeit','Zuschauer','Stadion','Schiri','Spielbericht','Verweise'])

print(heimspiele)

durchschnitt = heimspiele.select_dtypes(include='number').mean()
print(durchschnitt)
durchschnitt = heimspiele.tail(3).select_dtypes(include='number').mean()
print(durchschnitt)
#print(heimspiele.describe()) #Überblick
