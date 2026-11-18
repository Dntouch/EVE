# EVE – Neuronale Bewertungsrückmeldung

## Technische Bewertung und Codierung

Das BIOS kann zwei acht Bit breite Werte vergleichen. Es speichert tatsächliche
Ausgabe, technisches Ziel, Zahl übereinstimmender Bits und den ganzzahligen
Prozentwert getrennt. Nur der für den Versuch auf 0, 25, 50, 75 oder 100
quantisierte Erfolgswert wird neuronal codiert.

Die abgestufte Codierung ist one-hot: fünf getrennte sensorische Kontakte
stehen für die fünf Bewertungsstufen. Ein einfaches, nicht abgestuftes Signal
verwendet nur Kontakt 0. Die Zielinformation und einzelne richtige oder falsche
Bits sind nicht Bestandteil der Codierung.

## Rückmeldungswege

| Modus | Architektur |
|---|---|
| F0 | keine neuronale Übertragung |
| F1 | Feuerereignis eines eigenen sensorischen Kontakts |
| F2 | sensorische Kontakte mit deterministischer allgemeiner Direktverteilung |
| F3 | sensorische Kontakte über allgemeine Relais und deren Fanout |

F2/F3 entstehen vor der Handlung aus eigenem Seed, Kapazitätsparametern und
einer Liste reservierter Rollen. Weder Eligibility, M3-Kausalität noch
technisches Handlungsprotokoll werden bei der Zielwahl gelesen. Existierende
Verbindungen werden auf das konfigurierte Funktionsgewicht gesetzt; sonst
entstehen normale gerichtete Synapsen.

## Ereigniskontext

`feedback_fire` und `feedback_impulse` besitzen dieselbe neuronale Wirkung wie
reguläre Feuer- und Impulsereignisse. Die getrennten Arten transportieren nur
die Herkunft für Forschungsdiagnose und Persistenz. Feuert ein durch
Rückmeldung erreichter Empfänger, wird der Kontext über seine tatsächlichen
Ausgangssynapsen fortgeführt. Es gibt keine technische Sprungverbindung zu
markierten Neuronen.

Vor Verarbeitung jedes Rückmeldungsimpulses wird ausschließlich geprüft, wie
viele gültige, nicht durch dieselbe Rückmeldung erzeugte Eligibility-
Markierungen der Empfänger bereits besitzt. Gezählt werden Empfang mit keiner,
einer oder mehreren Markierungen. Löst der Impuls selbst ein Feuern und eine
Eligibility aus, trägt diese dauerhaft das Diagnosemerkmal
`feedback_origin`. Sie kann bei späteren Impulsen derselben Kaskade nicht als
vorausgegangene Handlungserfahrung gezählt werden.

Das Herkunftsmerkmal bewertet die Markierung nicht und verändert keine
Kandidaten. Es trennt nur zeitlich vorausgehende Aktivität von Aktivität, die
erst durch das Prüfsignal entstand.

## Zeit und Persistenz

Rückmeldungsfeuer können für einen zukünftigen Simulationszeitpunkt geplant
werden. Zeit und Einfügereihenfolge bestimmen die Verarbeitung. Bereits
geplante Eligibility-Abläufe werden am gleichen Zeitpunkt vor einem später
eingefügten Rückmeldungsereignis verarbeitet. Abgelaufene Markierungen werden
nicht wieder aktiviert.

Persistenzformat 7 speichert die neuen Metriken, Rückmeldungsereignisse und das
Herkunftsmerkmal aktiver Eligibility-Markierungen. Ein ausstehendes Ereignis
benötigt nach dem Laden keine technische Handlungs-ID oder Routentabelle, weil
sein neuronaler Eingang und alle Routensynapsen bereits Zustand des Netzwerks
sind. Formate 1 bis 6 bleiben lesbar.

## Zuordnungsgrenze

Lokale Eligibility-Generationen sind nur pro Neuron eindeutig. Gleiche Zahlen
an zwei Neuronen stellen keine gemeinsame Handlung dar. Z0 kann deshalb nur
zeitliche Überlappung feststellen. Ein neuronaler Grenzreiz in Z1 macht einen
Abschnitt beobachtbar, erzeugt aber noch keine geteilte Handlungsidentität.
Z2 benötigt einen eigenständigen neuronalen Kontextweg; eine technische
Handlungs-ID wird nicht als Ersatz in lokale Markierungen geschrieben.

