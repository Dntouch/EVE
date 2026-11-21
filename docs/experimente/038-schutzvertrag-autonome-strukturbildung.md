# C.0-Schutzvertrag für autonome Strukturbildung

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
- keine automatische Löschregel und kein globaler Lösungsscan.

## Ergebnis

In 12.800 zentralen Versuchen entstanden 166.261 Growth-Ereignisse ohne einen
einzigen unzulässigen Sender oder Empfänger. Alle 12.800 Netze blieben gültig.
Die 32-Neuronen-Variante besitzt nur 14 freie Verarbeitungsknoten und wird
deshalb nicht als kapazitiv äquivalent zu den größeren Varianten bewertet.
