# 0005 – Stammhirnentwicklung nach Forschungsstufe A.1

**Status:** Entwurf – gemeinsame Auswertung ausstehend

**Datum:** 2026-11-18

**Verantwortung:** offen

## Kontext

A.1 weist eine funktionale neuronale Kette von positiver angeborener
Verschaltung über Motoraktivität und neuronalen Abschluss bis zur sensorischen
Eigenrückmeldung nach. Die Forschungsdaten zeigen zugleich, dass die
Abschlussrate nicht monoton mit zusätzlichen Mechanismen steigt. Zufällige
Exploration kann den Abschlusskontakt häufiger treffen als die untersuchten
motorischen Gruppen. Eine technische Fähigkeit zum Abschluss ist damit noch
keine zuverlässige neuronale Entscheidung über das Ende einer Handlung.

## Entscheidungskriterien

- Kein Zielbyte und keine semantische Zuordnung im Stammhirn.
- Tatsächlicher neuronaler Ursprung jeder Daten- und Abschlussaktivität.
- Abschluss muss gegenüber einem zufälligen Treffer messbar abgegrenzt sein.
- Eigen- und Umweltrückmeldung bleiben getrennte Beobachtungen.
- Keine neue Lernregel ohne kontrollierte Vergleichsgruppen.
- Ereignisbudget, Persistenz und Reproduzierbarkeit bleiben erhalten.

## Betrachtete Optionen

### Option A – A.1-Mechanismen sofort als feste Architektur übernehmen

V2 mit Gewicht 3, E4, M2 und R1 werden zum dauerhaften Stammhirnstandard.

Dies stellt eine vollständige technische Kette bereit. Die Daten rechtfertigen
aber weder M2 als beste Schaltung noch die konkrete Gewichtung als allgemeine
Architektur. Zufällige Abschlüsse würden als erwünschtes Verhalten verfestigt.

### Option B – Varianten beibehalten und Abschlusskausalität isoliert untersuchen

Die Mechanismen bleiben einzeln konfigurierbare Forschungsvarianten. Eine
kurze Folgestufe misst, ob und wann ein Abschluss aus vorausgegangener
Motoraktivität entsteht, und vergleicht ihn mit zufälligen Treffern. Erst danach
wird über die Grundschaltung entschieden.

Dies verzögert die Lernstufe, schützt aber die Trennung zwischen technischer
Möglichkeit und neuronaler Funktion.

### Option C – Unmittelbar rückmeldungsabhängiges Lernen einführen

R1 oder R2 wird sofort mit einer neuen lokalen Ereignisspur und Verstärkungsregel
verbunden.

Damit könnte die Eigenrückmeldung früh genutzt werden. Die noch unklare
Abschlussursache würde jedoch Lern- und Handlungsmechanismus vermischen. Ein
Erfolg wäre anschließend nicht eindeutig zuzuordnen.

## Vorgeschlagene Entscheidung

Option B wird zur gemeinsamen Prüfung empfohlen. V2 mit Gewicht 3, E4, M1 bis
M3 und R1 bis R3 bleiben verfügbar, werden aber nicht als endgültige
Stammhirnarchitektur deklariert. Die nächste Untersuchung soll insbesondere
gezielte Motoraktivität, zufällige Abschlussaktivität und einen kausal
motorabhängigen Abschluss getrennt vermessen.

## Konsequenzen

### Positiv

- Die in A.1 geschaffene sensomotorische Infrastruktur bleibt nutzbar.
- Negative und überraschende Ergebnisse werden nicht durch neue Regeln
  verdeckt.
- Eine spätere Lernregel erhält besser definierte vollständige Handlungen.

### Negativ oder begrenzend

- Eine weitere isolierte Untersuchung ist nötig.
- Die gewählte angeborene Gewichtung 3 bleibt vorläufig ein Versuchsparameter.
- Ein dauerhaft autonomer Rückkopplungszyklus ist weiterhin offen.

## Offene Folgearbeit

- Abschlussrate bei gezielt kontrollierter Anzahl und Reihenfolge von
  Datenmotorereignissen messen.
- M1 bis M3 gegen identische Netz- und Explorationsverläufe vergleichen.
- Wiederholte Antwortphasen und dauerhafte Umweltzustände untersuchen.
- Erst anschließend eine minimale lokale Ereigniszuordnung spezifizieren.

## Nachweise und Verknüpfungen

- [Entwicklungsauftrag A.1](../experimente/005-entwicklungsauftrag-forschungsstufe-a1.md)
- [Forschungsbericht A.1](../experimente/005-forschungsbericht-forschungsstufe-a1.md)
- [Übergeordnetes Stammhirnkonzept](../konzept/EVE-Stammhirnkonzept.md)
- `research/results/research-stage-a1-results.csv`

## Überprüfung

Die Vorlage ist nach gemeinsamer Auswertung der A.1-Rohdaten zu entscheiden.
Sie wird spätestens überprüft, wenn eine Grundschaltung gegenüber identischer
Kontrollaktivität reproduzierbar einen motorabhängigen Abschluss erzeugt.
