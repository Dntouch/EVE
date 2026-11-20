# Technische Kausalanalyse – sensorische Kreditzuordnung B.5.7

## Produktionspfad

Ein gewöhnlicher Impuls wird zuerst als lokales G3-Ereignis gespeichert. Feuert
der Empfänger, erzeugt `Brainstem::create_eligibility()` aus der rückwärts
gelesenen lokalen Ereignisgeschichte einen begrenzten Kandidatensatz.
`active_phase` lässt nur Synapsen mit Aktivität in der aktuellen Phase zu; die
Darstellung `unique` entfernt Wiederholungen innerhalb dieses einen Eintrags.
Mehrere erhaltene Einträge können technisch weiterhin dieselbe Synapse
enthalten.

`Runtime::process_modulatory_feedback()` kodiert ausschließlich eine skalare
Polarität und sendet sie über reguläre, vorkonfigurierte Evaluationskontakte.
Ein Modulationsimpuls wirkt nur am tatsächlichen Empfänger seines lokalen
Kontakts. `Runtime::apply_local_modulation()` liest dort alle aktiven
Eligibility-Einträge, überspringt Feedback-Ursprünge und geschützte Synapsen
und wendet den lokalen Schritt auf jeden Kandidaten an. Hemmende Beiträge
invertieren bei beitragsbewusster Plastizität die Polarität. Der Kontakt ändert
weder Membran noch G3 und erzeugt kein Motorfeuern.

Da die Schleife Eintrag für Eintrag arbeitet, ist eine Folge 3→2→1 technisch
möglich. Der neue Test erzeugt sie gezielt. Im natürlichen B.5.7-Pfad war bei
allen 32.835 sensorischen Anwendungen genau ein Eintrag aktiv; nach der
Modulation wurde er verbraucht. Daher gab es weder überlappende ältere
Erfahrungen noch doppelte Änderungen.

## Diagnoseschnittstellen

`PlasticityObservation` meldet verhaltensneutral:

- Ereignisindex und Simulationszeit,
- lokalen Modulationskontakt, Sender, Empfänger und Polarität,
- Eligibility-Generation, berechnete Entstehungszeit, Ablauf, Stärke und
  vollständige Kandidatenliste,
- Kandidat, synaptischen Effekt, Gewicht vorher/nachher und tatsächliche
  Änderung.

Der Callback wird nicht persistiert und fließt nicht in Fingerprints ein. Ein
Regressionstest führt identische Läufe mit und ohne Callback bis zum exakt
gleichen Fingerprint fort. Die experimentelle Eligibility-Ersetzung arbeitet
nur auf expliziten Runtime-Kopien; sie wird niemals automatisch aufgerufen.

## Kausale Trennung

Für jede protokollierte sensorische Anwendung beginnt die Ablation unmittelbar
vor der Handlung aus einer vollständigen Runtime-Kopie. Eine Synapse ist:

- **zeitlich berechtigt**, wenn sie im angewendeten Eligibility-Eintrag steht;
- **funktional beteiligt**, wenn ihr Beitrag die nachfolgende Aktivität
  beeinflusst;
- **kausal notwendig**, wenn Gewicht null die tatsächliche Motorantwort ändert.

Diese Definition ergibt in den unabhängigen Fest-16-Gruppen 7.556 notwendige
und 1.888 nicht notwendige negative Änderungen. Die externe Zuordnung von
Generation zu Episode und Cue wird nur in CSV geschrieben und nie in der
neuronalen Regel verwendet.

## Gegenproben

Aus demselben Zustand werden natürliche, ausgewählte, verbleibende,
deduplizierte, neutrale und vollständig unmodulierte Kopien erzeugt. Natürlich,
ausgewählt und dedupliziert sind im beobachteten Ein-Eintrags-Pfad identisch.
Verbleibende, neutrale und unmodulierte Kopien führen die ausgewählte negative
Änderung nicht aus. Die Kopien laufen anschließend 1, 4, 8, 16, 32 und 64
natürliche Erfahrungen weiter.

Die unmittelbare Fortsetzung zeigt einen wichtigen Gegenbefund: Natürliche
negative Modulation stellt nach einer Erfahrung häufiger gemeinsame
Korrektheit her als ihre Unterdrückung (40/48 gegenüber 4/48). Eine pauschale
Filterregel würde damit notwendige Korrekturen entfernen.

## Persistenz und Gültigkeit

Aktive Eligibility und ausstehende Modulationsereignisse werden exakt
gespeichert und deterministisch fortgesetzt. Alle 637 Tests bestehen. Sämtliche
4.224 Netzläufe, 40.536 Diagnosekopien und 2.592 Fortsetzungsmessungen waren
gültig. S4 ist im Standardzustand weiterhin deaktiviert.
