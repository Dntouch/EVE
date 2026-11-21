# C.0-Schutzvertrag für autonome Strukturbildung

Der Vertrag ist eine außerhalb des neuronalen Lernzustands verwaltete
Versuchsvoraussetzung. Seine Prüfung erfolgt unabhängig von Funktions- und
Lernmetriken gemäß
[Architekturentscheidung 0037](../entscheidungen/0037-schutzvertrag-ist-voraussetzung-nicht-lernziel.md).

## Rollen

| Rolle | Growth-Sender | Growth-Ziel | Bedeutung |
|---|---:|---:|---|
| `innate_sensory` | nein | nein | angeborene Sensorkanäle |
| `innate_motor` | nein | nein | angeborene Motorkontakte |
| `evaluation_feedback` | nein | nein | technische Bewertung und Feedback |
| `protected_brainstem` | nein | nein | angeborene Steuerfunktionen |
| `free_processing` | ja | ja | frei rekrutierbare Verarbeitung |
| `transition_contact` | ja | ja | ausdrücklich erlaubter Übergang |

Jedes Neuron erhält eine persistierte semantische Rolle. Die Prüfung hängt
nicht von fest codierten Neuron-IDs ab. Eine unvollständige Rollenliste wird
abgelehnt.

Wenn autonome interne Exploration und Growth gleichzeitig aktiv sind, ist
dieser vollständige und sichere Vertrag eine Runtime-Invariante. Ungeschützte
Erstkonfiguration, spätere Aktivierung von Growth oder Exploration, Entfernen
des Vertrags sowie Laden eines ungeschützten kombinierten Zustands werden
abgelehnt. Reine historische Growth- oder Explorationsmodi bleiben davon
unberührt.

## Erlaubte Verbindungstypen

Der sichere Standard erlaubt ausschließlich:

- `free_processing → free_processing`,
- `free_processing → transition_contact`,
- `transition_contact → free_processing`,
- `transition_contact → transition_contact`.

Alle Richtungen mit Sensorik, Motorik, Bewertung/Feedback oder geschütztem
Stammhirn sind gesperrt. Der Vertrag wird unmittelbar bei jeder lokalen
Growth-Kandidatenauswahl geprüft. Exploration wählt bei aktivem Vertrag nur
`free_processing`-Knoten.

## Sicherheitsgrenzen

- autonome Exploration: höchstens acht Aktivierungen pro Versuch,
- höchstens eine autonome Aktivierung je auslösendem Feuerereignis,
- kein rekursiver Explorationstrigger durch das explorative Feuer,
- lokale räumliche Reichweite zwei,
- Growth behält seine lokale, vom Ausgangsgrad abhängige Wahrscheinlichkeit,
- die kombinierte Aktivierung ist ohne gültigen Vertrag unmöglich,
- keine automatische Löschregel und kein globaler Lösungsscan.

## Unabhängige Freigabe

Vor dem C.0-Lernlauf prüft ein eigener Preflight zwölf Positiv- und
Negativfälle. Dazu gehören bewusst provozierte Growth-Versuche mit geschützten
Sendern und ausgeschöpften zulässigen Zielen. Erst wenn alle Prüfungen
bestehen, startet der Forschungsrunner die allgemeinen Regressionen und die
Lernexperimente. Die null Verletzungen in natürlichen Läufen sind hiervon
getrennte Beobachtungsevidenz.

Neuronale Aktivität, Growth und Plastizität besitzen keine Berechtigung, den
Vertrag zu verändern. Der Vertrag schützt zugleich ausdrücklich keine
erfolgreichen freien Synapsen vor späterer Reorganisation.

## Ergebnis

In 12.800 zentralen Versuchen entstanden 166.261 Growth-Ereignisse ohne einen
einzigen unzulässigen Sender oder Empfänger. Alle 12.800 Netze blieben gültig.
Fünf gezielte Regressionstests prüfen Erstaktivierung, unsichere
Richtungsmatrix, spätere Konfigurationsänderung, Vertragsentfernung und
Persistenz.
Die 32-Neuronen-Variante besitzt nur 14 freie Verarbeitungsknoten und wird
deshalb nicht als kapazitiv äquivalent zu den größeren Varianten bewertet.
