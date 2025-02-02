import math

def poisson_probability(average_goals, actual_goals):
    """Berechnet die Poisson-Wahrscheinlichkeit."""
    return (average_goals**actual_goals * math.exp(-average_goals)) / math.factorial(actual_goals)

def calculate_match_probabilities(team1_avg_goals, team2_avg_goals):
    """Berechnet die Wahrscheinlichkeiten für alle Torkombinationen."""
    match_probabilities = {}
    for team1_goals in range(10):
        for team2_goals in range(10):
            team1_prob = poisson_probability(team1_avg_goals, team1_goals)
            team2_prob = poisson_probability(team2_avg_goals, team2_goals)
            match_prob = team1_prob * team2_prob
            match_probabilities[(team1_goals, team2_goals)] = match_prob
    return match_probabilities

def calculate_match_outcomes(match_probabilities):
    """Berechnet die Wahrscheinlichkeiten für die Spielausgänge."""
    home_win_prob = 0
    draw_prob = 0
    away_win_prob = 0

    for (team1_goals, team2_goals), probability in match_probabilities.items():
        if team1_goals > team2_goals:
            home_win_prob += probability
        elif team1_goals == team2_goals:
            draw_prob += probability
        else:
            away_win_prob += probability

    return home_win_prob, draw_prob, away_win_prob

def calculate_draw_no_bet(home_win_prob, draw_prob, away_win_prob):
    """Berechnet die Draw No Bet Wahrscheinlichkeiten."""
    home_win_no_draw = home_win_prob / (home_win_prob + away_win_prob) if (home_win_prob + away_win_prob) > 0 else 0
    away_win_no_draw = away_win_prob / (home_win_prob + away_win_prob) if (home_win_prob + away_win_prob) > 0 else 0
    return home_win_no_draw, away_win_no_draw

def print_match_probabilities(match_probabilities):
    """Gibt die Wahrscheinlichkeiten für jede Torkombination aus."""
    for (team1_goals, team2_goals), probability in match_probabilities.items():
        print(f"Team 1: {team1_goals} Tore, Team 2: {team2_goals} Tore: {probability:.4f}")

def print_match_outcomes(home_win_prob, draw_prob, away_win_prob):
    """Gibt die Wahrscheinlichkeiten für die Spielausgänge aus."""
    print("\nWahrscheinlichkeiten für Spielausgänge:")
    print(f"Heimsieg: {home_win_prob:.4f}")
    print(f"Unentschieden: {draw_prob:.4f}")
    print(f"Auswärtssieg: {away_win_prob:.4f}")

def print_draw_no_bet(home_win_no_draw, away_win_no_draw):
    """Gibt die Draw No Bet Wahrscheinlichkeiten aus."""
    print("\nDraw No Bet Wahrscheinlichkeiten:")
    print(f"Heimsieg (Draw No Bet): {home_win_no_draw:.4f}")
    print(f"Auswärtssieg (Draw No Bet): {away_win_no_draw:.4f}")

if __name__ == "__main__":
    team1_avg_goals = 0.9  # Beispielwerte
    team2_avg_goals = 0.6  # Beispielwerte

    match_probabilities = calculate_match_probabilities(team1_avg_goals, team2_avg_goals)
    #print_match_probabilities(match_probabilities)

    home_win_prob, draw_prob, away_win_prob = calculate_match_outcomes(match_probabilities)
    print_match_outcomes(1/home_win_prob, 1/draw_prob,1/ away_win_prob)

    home_win_no_draw, away_win_no_draw = calculate_draw_no_bet(home_win_prob, draw_prob, away_win_prob)
    print_draw_no_bet(1/home_win_no_draw, 1/away_win_no_draw)
