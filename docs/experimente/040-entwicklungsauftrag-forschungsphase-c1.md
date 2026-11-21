# Entwicklungsauftrag 040 – Forschungsphase C.1

## Neuronale Reorganisation, natürliche Plastizität und das Schicksal neu entstandener Verbindungen

**Projekt:** EVE – Emergent Virtual Evolution
**Forschungsphase:** C – Neuronale Selbstorganisation
**Forschungsstufe:** C.1
**Branch:** `codex/research-phase-c1-natural-path-reorganization`
**Technologie:** C++17, vollständig ereignisgesteuert
**Status:** kausale Untersuchung; keine neue Produktions-Lernregel freigegeben

## Forschungsfrage

Warum verlieren natürlich entstandene funktionale Verbindungen bei weiterer
Erfahrung ihre Wirkung, und welche Bedeutung haben diese Veränderungen für das
spätere Fähigkeitsrepertoire?

Gleichberechtigt zu prüfen sind fehlzugeordnete Plastizität, funktional
angemessene Reorganisation sowie architektonische oder methodische Grenzen.
Gemischte und nicht entscheidbare Befunde bleiben zulässig.

## Vier strikt getrennte Bewertungen

1. **A – Integrität:** War die Veränderung nach dem vorab festgelegten,
   extern verwalteten Schutzvertrag zulässig?
2. **B – Kausalität:** Welche Aktivität, Eligibility, Modulation und konkrete
   Gewichtsschreibung verursachten Veränderung und Funktionsverlust?
3. **C – Situative Angemessenheit:** War die Veränderung für die tatsächlich
   erlebte Situation nützlich, schädlich, neutral, kontextabhängig oder nicht
   entscheidbar?
4. **D – Langzeitwirkung:** War sie für spätere Erfahrungen und Fähigkeiten
   nützlich, schädlich, neutral, kontextabhängig oder nicht entscheidbar?

Es wird keine Gesamtpunktzahl gebildet. Integrität ist keine Erfolgsmetrik,
und spätere Erfolge oder Misserfolge verändern die Zulässigkeitsbewertung
nicht rückwirkend.

## Verbindliche Reihenfolge

1. Schutzrollen und Richtungen konfigurieren.
2. Eigenständigen Sicherheits-Preflight ausführen und bei Fehler abbrechen.
3. Allgemeine Regressionstests ausführen.
4. B.5.11, B.5.12, B.5.13 und C.0 deterministisch reproduzieren.
5. Erst danach C.1-Lernexperimente starten.

## Populationen

Die 19 funktionalen C.0-Fälle werden vollständig rekonstruiert. Zusätzlich
wird jede neue Growth-Verbindung zum Entstehungszeitpunkt registriert – vor
Kenntnis ihres späteren Gewichts oder Nutzens. Detaillierte Kausalanalysen
folgen der präregistrierten Regel `Gewicht >= Feuerschwelle` plus positiver
Einzelablationsnachweis; eine nachträgliche Auswahl günstiger Verläufe ist
ausgeschlossen.

Discovery und unabhängige Bestätigung verwenden je 64 Seeds pro
Aufgabenfamilie, Zuordnung und Netzgröße. Untersucht werden 32, 128, 256, 512
und 2.048 Neuronen; 32 wird als Kapazitätsrandfall ausgewiesen.

## Ereigniskette und lokales Informationsinventar

Für historische Fälle werden Entstehung, Aktivierung, Eligibility,
Modulation, die Gewichtsfolge 0→1→2, der erste kausale Funktionsbeitrag, die
nächste Handlung und Konsequenz, die konkrete 2→1-Schreibung sowie spätere
Erfahrungen erfasst. Zu jeder kritischen Schreibung gehören Sender,
Empfänger, Effekt, Membran, Refraktärzustand, Konkurrenz, Eligibility-Ursprung
und -zeit, Kandidatenmenge, Modulation, Motorantwort und tatsächliches
Schreibergebnis.

Lokal verfügbar sind nur aktuelle Impulse und Zustände, lokale Gewichte,
Membran, Refraktärzustand, Eligibility, Modulation, lokale Zeit und lokale
Konkurrenzsignale. Aufgabenlabels, richtige Zielhandlung, spätere
Kausalanalyse, Angemessenheitsklasse und späterer Lernerfolg gelangen nicht
in den neuronalen Mechanismus.

## Kausalmatrix

Alle Varianten starten aus vollständigen identischen Zustandskopien und
erhalten die natürliche Konsequenz ihrer tatsächlich ausgeführten Handlung.

| Variante | Intervention |
|---|---|
| W0 | unveränderte natürliche Fortsetzung |
| W1 | nur die ausgewählte kritische 2→1-Schreibung ausgelassen |
| W2 | identische unveränderte Kontrollkopie |
| W3 | alle weiteren Gewichtsschreibungen unterdrückt |
| W4 | ausgewählten Kandidaten aus lokaler Eligibility entfernt |
| W5 | lokale Modulationsschreibungen am betroffenen Empfänger unterdrückt |
| W6 | konkurrierende Eingänge am betroffenen Empfänger isoliert |
| W7 | weiteres Growth unterdrückt |
| W8 | weitere interne Exploration unterdrückt |
| W9 | gemeinsame Intervention W4+W6+W7+W8 |

Die Eingriffe sind ausschließlich in Forschungszustandskopien zulässig.

## Langzeit- und Fähigkeitsprüfung

W0 und W1 werden nach 0, 1, 2, 4, 8, 16, 32, 64, 128 und 256 weiteren
Erfahrungen verglichen. Erfasst werden erneute Verstärkung, weitere
Abschwächung, Funktionswiederkehr, alternative Pfade, Pfadwiederverwendung,
Repertoire, Retention, Wiedererwerb und Transfer. Eine funktionale Synapse ist
weder automatisch eine Erinnerung noch eine vollständige Fähigkeit.

Die Aufgabenmatrix umfasst einfache sensorisch-motorische Zuordnung,
konkurrierende Beziehungen, P→Q/P→R, A→B, A→B→A, gemischte Erfahrung,
Retention, Wiedererwerb, Transfer und B.5.12-Lernkontrollen.

## Statistik und Nachweise

Berichtet werden absolute Ereigniszahlen, vollständige Kurven, gepaarte
Kontrollen, getrennte Netzgrößen und seed-geclusterte 95-%-Bootstrap-Intervalle
mit 1.000 Resamples. Rohdaten bleiben maschinenlesbar. Persistenz wird mit
tatsächlich ausstehenden Ereignissen geprüft.

## Nicht freigegeben

- pauschaler Gewichtsschutz, Einfrieren oder automatischer Rollback,
- zielwissensabhängiges Growth oder Aufgabenlabels im Lernmechanismus,
- automatisches Verstärken neuer Synapsen,
- periodische globale Scans,
- eine neue Lernregel ohne lokalen Kausalnachweis,
- Produktionsaktivierung von Growth, Exploration oder S4-d16,
- nachträgliche Änderung des Schutzvertrags aufgrund von Lernergebnissen.

## Lieferumfang und Abschluss

Runner, Präregistrierung, Sicherheitsgate, historische Rekonstruktion,
prospektive Registrierung, Eligibility-/Modulationsanalyse, W0–W9,
Angemessenheits- und Langzeitanalyse, verteilte Reorganisation,
Mechanismen-/Größenvergleich, unabhängige Bestätigung, Ressourcen- und
Persistenztest, historische Reproduktion, Rohdaten, Forschungsbericht,
technische Kausalanalyse, Vier-Dimensionen-Bewertung,
Architekturentscheidung, C.2-Entscheidungsvorlage und explizite
Merge-Empfehlung sind verpflichtend.

Leitlinie: Wir schützen die Voraussetzungen neuronaler Entwicklung, nicht
jeden erreichten neuronalen Zustand. Lernen wird anhand überprüfbarer Folgen
bewertet, nicht anhand der Unveränderlichkeit seiner Verbindungen.
