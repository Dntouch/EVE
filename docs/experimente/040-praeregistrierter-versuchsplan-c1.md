# Präregistrierter Versuchsplan C.1

> **Transparenzhinweis vom 21. November 2026:** Dieser Plan dokumentiert die
> ursprüngliche Präregistrierung. H0–H3, die funktionalen
> Alternativpfad-Ablationen und die Diagnose jedes Schritts 0–256 wurden erst
> nach der methodischen Kritik ergänzt. Sie sind post-hoc Korrekturen und
> werden nicht rückwirkend als präregistriert ausgegeben.

## Geltungsbereich

Dieser Plan wird vor der Interpretation der C.1-Ergebnisse festgeschrieben.
Er operationalisiert Auftrag 040, ohne eine neue Lernregel zu genehmigen.

## Gate und Abbruchregel

`eve_structural_safety_preflight` muss vor Tests, Regression und Lernläufen
vollständig bestehen. Jede fehlgeschlagene Prüfung beendet den Lauf. Weder
Funktionsbildung noch Lernerfolg kann einen Integritätsfehler kompensieren.

## Stichprobe

- historische Population: alle 19 C.0-S3-Fälle,
- Discovery: Seedbereich ab 500.000,
- Bestätigung: unabhängiger Seedbereich ab 700.000,
- 64 Seeds je Aufgabenfamilie, Zuordnung und Netzgröße,
- Größen: 32, 128, 256, 512, 2.048 Neuronen,
- Aufgaben: die zehn in C.0 verwendeten Aufgabenfamilien.

Jede Growth-Synapse wird bei Entstehung registriert. Die präregistrierte
Auswahlregel für W0–W9 lautet: Sie erreicht am Ende der ersten 16 Erfahrungen
die Feuerschwelle und ihre Einzelablation entfernt die direkte Zielaktivierung.

## Primäre Endpunkte

- Gewicht und direkte Funktion nach 1 und 256 Folgeerfahrungen,
- Funktionswiederkehr bis Erfahrung 256,
- Unterschied W0/W1 in kumulativ korrekten Erfahrungen,
- Unterschied W0/W1 im Fähigkeitsrepertoire,
- alternative funktionale Eingänge am selben Ziel,
- getrennte A/B/C/D-Klassifikation.

## Sekundäre Endpunkte

Growth-Zahl, Gewicht 1, Feuerschwelle, Eligibility-Alter und -Kandidaten,
Modulationsvorzeichen, Membran, Refraktärzustand, tatsächliche Motorantwort,
positive/negative Schriften, Ereignisse, Speicher und Laufzeit.

## Interventionen und Auswertung

W0–W9 sind in Auftrag 040 definiert. Abweichende Handlungen erhalten jeweils
ihre eigene natürliche Konsequenz. W2 muss W0 deterministisch reproduzieren.
W1 bleibt nur in der ersten Folgeerfahrung aktiv; danach laufen beide
Langzeitkopien natürlich weiter.

Die ursprünglich vorgesehene situative Ableitung aus W0/W1 erwies sich als
unzureichend, weil deren Intervention erst nach der Handlung greift. Dieser
Teil des Plans ist durch den folgenden, ausdrücklich post-hoc ergänzten
Korrekturblock methodisch ersetzt. Langzeitklassen basieren weiterhin auf
kumulativem Erfolg, Repertoire und verbleibender Funktion nach 256
Erfahrungen. Die Integritätsklasse wird nie daraus abgeleitet.

## Post-hoc Methodenkorrektur

Die ergänzte Handlungskausalität vergleicht aus identischem Ausgangszustand:

- H0 unveränderte natürliche Erfahrung,
- H1 diagnostische Synapsendeaktivierung vor der Erfahrung,
- H2 unveränderte Determinismuskontrolle,
- H3 kombinierte Ablation konkurrierender Eingänge.

Die jeweilige tatsächliche Handlung erhält ihre eigene natürliche Rückmeldung.
Handlungsbeitrag, Folgeeffekt der späteren Gewichtsschreibung und direkte
Zielaktivierung werden getrennt ausgewiesen. Unklare Fälle werden
`unentschieden`, nicht automatisch neutral.

Alternative Eingänge werden strukturell, potenziell funktional, kontrolliert
funktional, neuronal kausal und fähigkeitskausal getrennt. Einzel- und
Kombinationsablationen sind erforderlich, bevor von funktionaler
Reorganisation gesprochen wird.

Die direkte Funktion wird nach jedem Schritt 0–256 in einer diagnostischen
Zustandskopie geprüft. Erfasst werden erster erneuter Nachweis, Dauer,
Häufigkeit, erneuter Verlust und Endzustand. Die Schwellenwerte des
ursprünglichen Plans bleiben unverändert.

## Statistik

Pro präregistrierter Gruppe werden Mittelwerte und seed-geclusterte
Perzentil-Bootstrap-Intervalle (95 %, 1.000 deterministische Resamples)
ausgegeben. Discovery und Bestätigung, Größen und Zuordnungen bleiben
getrennt. Es gibt keine zusammengesetzte Erfolgszahl.

## Aussagegrenzen

Einzelne Synapsen sind keine vollständigen Erinnerungen. Ein kurzfristiger
Funktionsverlust ist weder automatisch schädlich noch langfristig produktiv.
Nur beobachtete Gegenproben dürfen diese Bewertungen tragen. Extern
rekonstruierte Kausalität gilt nicht als lokal vorhandenes Wissen.
