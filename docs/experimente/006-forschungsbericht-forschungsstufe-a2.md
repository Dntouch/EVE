# Forschungsstufe A.2 – Kausalität neuronaler Handlungen und Abschlüsse

## Kurzfassung

Forschungsstufe A.2 trennt unmittelbare explorative Treffer von regulär
erreichter Motoraktivität und verfolgt reale Schaltungsimpulse bis zum
Abschlusskontakt. Die primäre Kausalitätsmatrix umfasst 224 Läufe mit 32 Seeds:
M0 als K0 sowie M1 bis M3 jeweils aktiv (K1) und mit identischer, durch Gewicht
0 gezielt unterbrochener Topologie (K2).

Die eingeschränkte Exploration E4-neu wählte in allen 1.010 primären und
wiederholten, direkt geprüften
Antwortphasen keinen Datenkontakt, Abschlusskontakt oder Motorschaltungsrelay
unmittelbar aus. Die A.1-Exploration E4-alt traf in sechs Vergleichsläufen zwei
Datenkontakte und einmal den Abschlusskontakt direkt; ihre zwei Abschlüsse
erfolgten beide ohne vorausgegangene Datenaktivität.

Nur M3 lieferte einen positiven Kausalitätsbefund. Bei aktiver Schaltung waren
7 von 32 Antworten vollständig; alle sieben Abschlüsse folgten Datenaktivität
und ließen sich über tatsächliche Akkumulator- und Abschlusssynapsen
rekonstruieren. Bei unterbrochenem M3 entstanden drei zufällige Abschlüsse,
aber kein nachgewiesener Schaltungsweg. Bedingt auf Datenaktivität betrug
`P(Abschluss nach Daten | Datenaktivität)` für M3/K1 7/22 = 31,8 %, für M3/K2
1/22 = 4,5 %.

M1 und M2 erzeugten in der Matrix keine vollständige Antwort. Die Diagnose
zeigt, dass ihre Wege vorhanden und aktiv waren, relevante Empfänger aber
refraktär waren: M1 verzeichnete 35 Schaltungsimpulse, die sämtlich durch die
Refraktärregel am Feuern gehindert wurden. M2 verzeichnete 53
Schaltungsimpulse, 18 relaisauslösende und 35 refraktär unterdrückte Impulse.
Die bestehende Refraktärregel wurde nicht verändert.

M3 ist damit die beste der untersuchten Grundlagen für spätere
rückmeldungsabhängige Forschung, aber noch nicht stabil genug für eine
endgültige Architekturentscheidung. Im fortgeführten Zustand trat der
nachgewiesene Abschluss nur in der ersten von acht Wiederholungen auf; weitere
Antwortphasen blieben wegen fortbestehender neuronaler Zustände inaktiv.

## A. Technische Änderungen

### A.1 Konfigurationsabhängige Explorationsbereiche

`processing_neurons_only` bildet die Ausschlussmenge aus der tatsächlich
konfigurierten Laufzeit:

- sensorische Kontakte 0 bis 7,
- konfigurierter Antwortreiz,
- der aus Antwortreiz und erstem Motorkontakt abgeleitete reservierte
  Rückmeldebereich,
- alle Daten- und Abschlusskontakte,
- alle Relais der angeborenen Grundverschaltung,
- Relais beziehungsweise Akkumulator der Motorschaltung,
- zusätzliche, explizit für Variantenvergleiche gemeinsam reservierte
  Neuronen.

Die IDs von Motor-, Pfad- und Schaltungsneuronen werden nicht vorausgesetzt,
sondern aus den jeweiligen Konfigurationsergebnissen übernommen. Der Ausschluss
betrifft nur die direkte Explorationsauswahl. Dieselben Neuronen empfangen
weiterhin Impulse, verändern Membran- und Refraktärzustand und können regulär
feuern.

Für den M0-bis-M3-Vergleich wird die Vereinigung aller möglichen M2- und
M3-Relais zusätzlich reserviert. Dadurch bleibt die Explorationsfolge bei
gleichem Seed über alle Schaltungsvarianten identisch.

### A.2 Unterbrechbare Motorschaltungen

M1 bis M3 melden nun die konkreten Synapsen ihrer Schaltung. K2 legt exakt
dieselbe Topologie wie K1 an, setzt die Gewichte der untersuchten Strecke aber
auf 0. K1 und K2 unterscheiden sich damit nicht in Relaisauswahl oder
zusätzlicher Ereignisstruktur, sondern in der funktionalen Übertragung.

M0 legt weiterhin keine Motorschaltung an. M1 besitzt acht direkte Kanten, M2
acht Daten-zu-Relay- und vier Relay-zu-Abschluss-Kanten, M3 acht
Daten-zu-Akkumulator- und eine Akkumulator-zu-Abschluss-Kante.

### A.3 Ereignisdiagnose und Kausalitätsklassifikation

Eine optionale Beobachtung erfasst für synaptische Impulse:

- globale Ereignisreihenfolge,
- Synapse, Sender und Empfänger,
- tatsächlich angewandtes Gewicht,
- Membranpotenzial vor und nach dem Eingang,
- Refraktärzustand vor und nach dem Eingang,
- ausgelöstes oder refraktär unterdrücktes Feuern.

Die Diagnose speichert keine Zeitstempel oder Spuren in Neuronen oder Synapsen.
Ohne registrierten Beobachter werden die zusätzlichen Zustandskopien nicht
erzeugt. Die Kausalitätsanalyse beginnt bei tatsächlich beobachteten
Datenkontakten und akzeptiert einen nachgewiesenen Schaltungsweg nur, wenn reale
Schaltungsimpulse nacheinander feuernde Empfänger bis zum tatsächlich
feuernden Abschlusskontakt erreichen.

Getrennt ausgewiesen werden fehlende Antworten, technische Abbrüche,
Abschlüsse ohne Daten, Abschlüsse nach Daten und nachgewiesene
Schaltungsabschlüsse.

## B. Versuchsaufbau

Gemeinsame Parameter:

| Parameter | Wert |
|---|---:|
| Neuronen | 256 |
| reguläre Startsynapsen | 1.024 |
| Geometrie | 8 × 8 × 4 |
| reguläres Startgewicht | 0 |
| angeborene Pfade | V2, Gewicht 3 |
| Feuergrenze | 3 |
| Refraktärereignisse | 16 |
| Lernen und Wachstum | deaktiviert |
| Ereignisbudget | 100.000 |
| primäre Exploration | E4-neu, 16 Aktivierungen |
| primäre Eingabe | `AB` |

Die Primärmatrix verwendet die sechs etablierten und 26 zusätzliche feste
Seeds. Netz, Motorzuordnung, angeborene Topologie, Exploration und
Motorschaltung besitzen getrennte Zufallszustände. Die Explorationsfolge ist
für K0, K1 und K2 identisch.

Der Runner `scripts/run_research_stage_a2.sh` erzeugt:

- `research-stage-a2-parameters.csv`,
- `research-stage-a2-exploration.csv` mit 30 Läufen,
- `research-stage-a2-causality.csv` mit 224 Läufen,
- `research-stage-a2-diagnostics.csv` mit einem vollständigen M3-Beispiel,
- `research-stage-a2-repetitions.csv` mit 768 Antwortphasen,
- `research-stage-a2-feedback.csv` mit 48 Zwei-Phasen-Läufen,
- `research-stage-a2-performance.csv` mit 64 Diagnosevergleichen.

## C. Phase I – Explorationsbereiche

| Variante | zulässige Neuronen | direkte Datentreffer | direkte Abschlusstreffer | Läufe mit Daten | Abschlüsse ohne Daten |
|---|---:|---:|---:|---:|---:|
| E0 | 208–210 | 0 | 0 | 3/6 | 0 |
| E4-alt, 16 | 256 | 2 | 1 | 2/6 | 2 |
| E4-neu, 8 | 208–210 | 0 | 0 | 3/6 | 0 |
| E4-neu, 16 | 208–210 | 0 | 0 | 3/6 | 0 |
| E4-neu, 32 | 208–210 | 0 | 0 | 3/6 | 0 |

E4-neu erfüllt die harte Fehlerbedingung: kein reservierter Motor- oder
Abschlusskontakt wurde direkt gewählt. Die Ergebnisse von E0 und allen drei
E4-neu-Stärken waren in dieser Stichprobe identisch. Die ausgewählten
allgemeinen Neuronen besitzen bei deaktiviertem Lernen und regulärem Gewicht 0
keinen funktionalen Ausgangspfad; ihre zusätzlichen Feuerereignisse erreichten
keinen Motorbereich. Die beobachteten sechs Datenereignisse entstanden aus
Eingabe und angeborener Grundverschaltung, nicht aus Exploration.

Damit ist Frage 1 negativ zu beantworten: Die eingeschränkte Exploration ist
technisch korrekt, führte unter diesen Bedingungen aber noch nicht aus sich
heraus zu zusätzlicher Motoraktivität.

## D. Phasen II und III – Abschlusskausalität

| Schaltung | Kontrolle | Daten in Läufen | Abschlüsse nach Daten | ohne Daten | nachgewiesener Weg | P(Abschluss nach Daten \| Daten) |
|---|---|---:|---:|---:|---:|---:|
| M0 | K0 | 22/32 | 1 | 2 | 0 | 4,5 % |
| M1 | K1 | 22/32 | 0 | 0 | 0 | 0,0 % |
| M1 | K2 | 22/32 | 1 | 2 | 0 | 4,5 % |
| M2 | K1 | 22/32 | 0 | 0 | 0 | 0,0 % |
| M2 | K2 | 22/32 | 1 | 2 | 0 | 4,5 % |
| M3 | K1 | 22/32 | 7 | 0 | 7 | 31,8 % |
| M3 | K2 | 22/32 | 1 | 2 | 0 | 4,5 % |

In jeder Zeile entstanden dieselben 35 Datenmotorereignisse. Direkte
Explorationstreffer waren immer 0. K0 und die drei K2-Gruppen reproduzierten
dieselben drei zufälligen Abschlüsse: zwei ohne und einen nach Datenaktivität.
Keiner war auf eine Motorschaltung zurückzuführen.

M3/K1 erzeugte sieben zusätzliche, vollständig nachgewiesene Abschlüsse. Die
Werte 0x60, 0x10, 0xc4, 0x81, 0x01 und 0xa0 zeigen mehrere neuronale
Motorfolgen; 0x60 trat zweimal auf. Zwischen erster Datenaktivität und Abschluss
lagen durchschnittlich 16 verarbeitete Ereignisse. Die Seeds waren 2026,
314159, 2, 3, 5, 53 und 83.

Der Diagnoseausschnitt für Seed 2026 zeigt beispielhaft drei Datenkontakte,
deren Impulse das Akkumulatorpotenzial von 0 über 1 und 2 bis zur Feuergrenze 3
erhöhen. Der Akkumulator feuert und überträgt Gewicht 3 an den Abschlusskontakt,
der daraufhin feuert. Das ist ein vollständiger realer Schaltungsweg.

M1 und M2 scheiterten nicht an fehlenden Verbindungen. M1 transportierte 35
positive Schaltungsimpulse; alle 35 wurden durch Refraktärität am Feuern
gehindert. M2 transportierte 53 Schaltungsimpulse, löste 18 Relayfeuerungen aus
und verzeichnete ebenfalls 35 refraktär unterdrückte Eingänge. M3 verteilte die
Aktivität zunächst im Akkumulator und konnte deshalb in sieben Fällen nach
mehreren Datenereignissen abschließen.

## E. Phase IV – Wiederholte Handlungen

Jede Kombination aus sechs etablierten Seeds, den Eingaben `AB` und `BA`, M0
bis M3 und acht Wiederholungen wurde einmal aus identisch gespeichertem Zustand
und einmal mit fortgeführtem neuronalen Zustand ausgeführt.

| Zustand | Schaltung | Antwortphasen | Phasen mit Daten | nachgewiesene Abschlüsse |
|---|---|---:|---:|---:|
| wiederhergestellt | M0 | 96 | 48 | 0 |
| wiederhergestellt | M1 | 96 | 48 | 0 |
| wiederhergestellt | M2 | 96 | 48 | 0 |
| wiederhergestellt | M3 | 96 | 48 | 32 |
| fortgeführt | M0 | 96 | 6 | 0 |
| fortgeführt | M1 | 96 | 6 | 0 |
| fortgeführt | M2 | 96 | 6 | 0 |
| fortgeführt | M3 | 96 | 6 | 4 |

Wiederhergestellte Ausgangszustände reproduzierten Antwort und
Zustandsfingerabdruck exakt. Bei fortgeführtem Zustand traten alle vier
M3-Abschlüsse in Wiederholung 1 auf; Wiederholungen 2 bis 8 erzeugten keine
Datenaktivität. Dieses Ergebnis gilt gleichermaßen für `AB` und `BA`.

M3 organisiert somit einzelne Handlungen kausal, ist mit der unveränderten
Refraktärphase aber noch kein stabil wiederholbarer Handlungsmechanismus. Die
Refraktärregel wurde weder zurückgesetzt noch verkürzt.

## F. Phase V – Eigenrückmeldung

Rückmeldung war in allen primären Kausalitätsläufen deaktiviert. In der
nachgelagerten Sechs-Seed-Prüfung schloss M3 zwei erste Antworten ab. R1 führte
deren tatsächliche Ausgaben als insgesamt fünf Sensorereignisse zurück und
erzeugte fünf zusätzliche reguläre Feuerereignisse. R0 erzeugte keine.

Die jeweils zweite Antwortphase schloss weder mit R0 noch mit R1 ab. Es gab
keine selbstverstärkende Schleife und keinen Abbruch. Die ursprüngliche
Motorfolge bleibt unverändert in der ersten Antwort dokumentiert. Damit
funktioniert die A.1-Eigenrückmeldung weiter, behebt aber erwartungsgemäß nicht
die Wiederholungsinstabilität und führt noch kein Lernen ein.

## G. Tests, Reproduzierbarkeit und Ressourcen

Die Testsuite umfasst jetzt 72 Tests. Neu geprüft werden unter anderem:

- vollständiger Ausschluss aller konfigurierten Reservierungen,
- normale synaptische Aktivierbarkeit ausgeschlossener Neuronen,
- reproduzierbare und schaltungsunabhängige Explorationsfolgen,
- exakte M0-bis-M3-Verbindungsinventare,
- identische K1/K2-Topologie bei unterbrochener Übertragung,
- getrennte Klassifikation von Nullbyte, fehlender Antwort und Abbruch,
- Rekonstruktion von M2- und M3-Schaltungswegen,
- Persistenz des integrierten neuronalen Zustands.

Alle 72 Tests bestehen. Ein zweiter vollständiger Forschungsdurchlauf erzeugte
für alle Konfigurationen, Ereignisfolgen, Antworten, Diagnosen und
Zustandsfingerabdrücke identische Werte. Nur Laufzeit und Durchsatz wichen ab.

Die 224 Primärläufe verarbeiteten 40.722 Ereignisse und 33.274 synaptische
Impulse. Die maximale Warteschlange enthielt 71 Ereignisse; kein Lauf brach ab.
Die Laufzeitdiagnostik schätzte 60.448 bis 60.512 Byte. Detaildiagnose benötigte
außerhalb des neuronalen Zustands durchschnittlich 14.336 Byte temporären
Speicher. Auf der Messmaschine stieg die mittlere Laufzeit eines M2-Falls von
etwa 10,0 auf 10,7 Mikrosekunden. Diese sehr kurzen Zeitwerte sind
maschinenabhängig; Diagnose an/aus erzeugte in allen 32 Paaren denselben
Zustandsfingerabdruck.

## H. Antworten auf die Abschlussfragen

1. **Erreicht eingeschränkte Exploration Motorik?** Nicht zusätzlich unter den
   untersuchten Gewichten; sie erzeugt isolierte Feuerungen, aber keine
   funktionale Verbindung aus allgemeinen Verarbeitungsneuronen.
2. **Kann eine Motorschaltung nach Daten abschließen?** Ja, M3 in 7 von 32
   Primärläufen.
3. **Ist die Schaltung kausal nachgewiesen?** Ja. Alle sieben M3/K1-Abschlüsse
   besitzen einen rekonstruierten Akkumulatorweg; M3/K2 besitzt keinen.
4. **Wie unterscheiden sich M0–M3?** M0 erzeugt selten zufällige Abschlüsse,
   M1 und M2 scheitern refraktär, M3 bündelt mehrere Datenereignisse erfolgreich.
5. **Welche Rolle spielt Zufall?** K0/K2 zeigen drei zufällige Abschlüsse, davon
   zwei ohne Daten. Sie sind von den sieben M3-Schaltungsabschlüssen getrennt.
6. **Ist die Organisation wiederholbar stabil?** Aus identischem Zustand ja,
   im fortgeführten Zustand nein.
7. **Funktioniert Eigenrückmeldung?** Ja, ohne Selbstverstärkung und ohne
   Auswirkung auf die fehlende zweite Antwort.
8. **Beste Grundlage für B?** M3 ist der einzige kausal belegte Kandidat, muss
   aber bis zur gemeinsamen Entscheidung als Forschungsvariante gelten.

## I. Forschungsgrenzen und Empfehlung

Es wurden keine neue Lernregel, Bewertung, Hemmung, Abschwächung, STDP oder
lokale Ereignisspur eingeführt. Die technische Diagnose ist nicht Teil des
neuronalen Zustands. Positive M3-Ergebnisse sind weder Lernen noch eine
inhaltlich sinnvolle Antwort.

Empfohlen wird, M3 als vorläufige Handlungsgrundlage für den Entwurf von
Forschungsstufe B zu verwenden und parallel die Interaktion zwischen
Antwortphasen und bestehender Refraktärregel gezielt zu untersuchen. M1 und M2
sollten nicht verworfen, aber ohne weitere Refraktäranalyse nicht als
Abschlussmechanismus übernommen werden. Eine endgültige Architekturentscheidung
bleibt der gemeinsamen Auswertung vorbehalten.

## Nachweise

- [Entwicklungsauftrag A.2](006-entwicklungsauftrag-forschungsstufe-a2.md)
- [Entscheidungsvorlage zu Forschungsstufe B](../entscheidungen/0006-vorlage-uebergang-zu-forschungsstufe-b.md)
- `research/results/research-stage-a2-*.csv`
