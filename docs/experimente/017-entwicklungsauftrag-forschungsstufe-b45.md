# Entwicklungsauftrag 017 – Forschungsstufe B.4.5

## Lokale Entscheidungsbindung und konkurrierende neuronale Aktivität

**Projekt:** EVE – Emergent Virtual Evolution
**Ausgangsbasis:** `codex/research-stage-b44-structural-learning`
**Sprache:** C++17
**Status:** Ursachenforschung mit kontrollierten Lernexperimenten

## Forschungsfrage

Warum kann EVE zwei Erfahrungen nacheinander behalten, aber nicht zuverlässig
lernen, wenn sie abwechselnd auftreten? Unabhängig zu prüfen sind zeitliche
Interferenz (H1), motorische Entscheidungskonkurrenz (H2) und fehlende lokale
Bindung entscheidungsrelevanter Eingänge (H3).

## Untersuchungsprogramm

Phase A protokolliert für jede diagnostische Lernentscheidung präsentierten
Reiz, sensorische und motorische Feuerereignisse, Ereignisreihenfolge, lokale
Eingänge, Eligibility-Kandidaten, tatsächliche Modulation und
Gewichtsänderungen. Korrekte/falsche Einzelhandlungen, doppelte und fehlende
Ausgaben werden getrennt ausgewiesen. Äußere Reizkennungen sind ausschließlich
in der Diagnose zulässig.

Phase B umfasst:

- K0: unverändertes G3,
- K1: unverändertes `active_phase`,
- K2: neutrale, negative oder vollständig ausgelassene Modulation bei
  doppelter Motoraktivierung,
- K3: kontrollierte Reizabstände, Eligibility-Dauern und Feedbackzeitpunkte,
- K4: Diagnose lokaler Eingangsbindung einschließlich Nullgewicht-Synapsen.

Erst ein identifizierter Engpass erlaubt bedingt C1 als lokale
Koinzidenzbindung oder C2 als ereignisgetriebene lokale motorische Konkurrenz.
Keine externe Reizkennung, Handlungs-ID, Zielverbindung oder technisch
erzwungene Auswahl darf Teil eines Mechanismus werden.

Alle zentralen Varianten durchlaufen Einzelreiz-, beide Sequenz-,
Alternierungs- und Mischaufgaben mit mindestens 64 gepaarten Seeds. Kandidaten
werden mit 64 unbenutzten Seeds bestätigt. Wachstum bleibt deaktiviert;
B.4.2–B.4.4 bleiben Regressionen.

## Erfolg

Das Hauptziel ist gemischtes Lernen mit mindestens 75 % eindeutiger Ausgaben
für X und Y, mindestens 15 Punkten gepaartem Vorteil gegenüber No-Learning,
positivem 95-%-Intervall, unabhängiger Reproduktion und ohne vollständigen
Verlust einer Beziehung. Mindestens 48/64 Seeds sollen beide Schwellen
gleichzeitig erreichen. Einseitige Gruppenmittel genügen nicht. Auch ein
negatives Ergebnis schließt die Forschungsstufe ab.

## Lieferumfang

Technische Konkurrenz- und Zeitdiagnose, K0–K4-Matrix, begründete
C1/C2-Entscheidung, Ereignis-/Gewichtsprotokolle, Verwechslungsmatrizen,
Seedwerte, Kausalvergleiche, Statistik, Regressionen, Ressourcen, Persistenz,
Forschungsbericht und Entscheidungsvorlage B.4.6.
