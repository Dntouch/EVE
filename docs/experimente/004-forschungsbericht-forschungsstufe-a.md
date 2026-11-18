# Forschungsstufe A – Neuronale Ausgabe und eigenständige Antwortbildung

## Kurzfassung

Die motorische Ausgabeschnittstelle ist technisch funktionsfähig und vom
neuronalen Kern getrennt. Acht binäre Datenkanäle, ein eigener neuronaler
Abschlusskontakt und ein unabhängiger Antwortreiz sind deterministisch an
vorhandene Neuronen angebunden. Feuerereignisse werden nur während einer
Antwortphase ausgewertet; sie verändern weder das neuronale Feuern noch die
synaptische Verarbeitung.

In 249 Forschungsversuchen entstand jedoch keine gültig abgeschlossene
neuronale Antwort. In allen 153 isolierten Läufen mit deaktiviertem Lernen und
Wachstum blieben die motorischen Kanäle inaktiv. In den 96 getrennt
gekennzeichneten Läufen mit den vorhandenen Lern- und Wachstumsregeln traten in
10 Läufen Datenkanalereignisse auf, aber der Abschlusskontakt feuerte nie.

Damit ist die zentrale Forschungsfrage differenziert zu beantworten: EVE kann
neuronale Aktivität technisch nach außen übertragen, sobald ein angebundenes
Neuron feuert. Das bestehende Netzwerk erzeugt unter den untersuchten
Bedingungen aber noch keine vollständige eigenständige Antwort.

## A. Technische Umsetzung

### Motorische Kontakte

Die Klasse MotorOutput im Stammhirn verwaltet:

- acht binäre Ausgangskanäle,
- eine konfigurierbare Kontaktgruppengröße,
- einen separaten Abschlusskontakt,
- die Aktivierungsreihenfolge und auslösenden Neuronen,
- die Zustände no_response, completed und aborted.

Untersucht wurden die vorgegebenen Varianten:

| Variante | Kontakte je Datenkanal | Datenkontakte | Abschlusskontakte |
|---|---:|---:|---:|
| M1 | 1 | 8 | 1 |
| M2 | 4 | 32 | 1 |
| M3 | 8 | 64 | 1 |

Die Zuordnung wird aus einem eigenen, konstanten Zuordnungs-Seed erzeugt. Sie
ist unabhängig vom Seed der Netzinitialisierung. Sensorneuronen 0 bis 7 und das
Neuron 8 des Antwortreizes sind von allen motorischen Kontakten ausgeschlossen.
Die Gruppen überlappen nicht.

### Antwortphase

Das BIOS startet eine klar abgegrenzte Antwortphase. Optional feuert dabei
Neuron 8 als technischer Antwortreiz. Dieser Reiz enthält weder Zielwert noch
Eingabedaten. Danach verarbeitet die vorhandene Ereigniswarteschlange nur
tatsächlich auftretende neuronale Ereignisse.

Eine Antwort ist ausschließlich gültig, wenn der neuronale Abschlusskontakt
feuert. Ein abgeschlossenes 0x00 bleibt dadurch von fehlender Aktivität
unterscheidbar. Ein konfigurierbares Ereignislimit führt zum Zustand aborted
und erfindet keine Ersatzantwort.

### Persistenzentscheidung

Neuronale Zustände dürfen nur außerhalb einer aktiven Antwortphase gespeichert
werden. Die Motorzuordnung ist technische Versuchskonfiguration und wird nach
dem Laden mit denselben Parametern reproduzierbar erneut erzeugt. Ein
Speicherversuch während einer aktiven Antwortphase wird ausdrücklich
abgewiesen. Damit werden keine unvollständigen Antworten stillschweigend
wiederhergestellt.

## B. Versuchsaufbau

Die Rohdaten liegen in:

- research/results/research-stage-a-results.csv
- research/results/research-stage-a-assignments.csv

Verwendet wurden drei Netz-Seeds (42, 2026, 20261118), alle drei
Kontaktvarianten und die im Auftrag vorgegebenen Eingaben. Die primären
isolierten Versuche deaktivierten Lernen und Wachstum. Zusätzlich wurden
getrennte Läufe mit den bereits vorhandenen Regeln C5+A1 durchgeführt:
Refraktärphase 16, maximales Gewicht 1, Lernen und Wachstum aktiviert.

| Modus | Zeilen | gültige Antworten | Läufe mit Datenkanalaktivität |
|---|---:|---:|---:|
| isoliert | 153 | 0 | 0 |
| vorhandene Regeln | 96 | 0 | 10 |
| **gesamt** | **249** | **0** | **10** |

## C. Ergebnisse A1 bis A5

### A1 – spontane Ausgangsaktivität

Alle neun Kombinationen aus drei Seeds und M1 bis M3 blieben ohne
Ausgangsaktivität. Ohne eingehendes Ereignis erzeugt die ereignisgesteuerte
Architektur erwartungsgemäß keine spontanen Feuerereignisse.

### A2 – sensorische Eingaben ohne Antwortreiz

Die acht Eingaben 00, 01, 10, 11, A, B, AB und BA wurden mit drei Seeds
verarbeitet. Nach dem vollständigen Abklingen der Eingabe entstand in keinem
der 24 isolierten Läufe eine motorische Aktivität oder ein Abschluss.

Dieses Ergebnis bestätigt zugleich die Phasentrennung: Während der
Eingabeverarbeitung auftretende Feuerungen werden nicht nachträglich als
Antwort übernommen.

### A3 – expliziter Antwortreiz

Der Antwortreiz erzeugte im isolierten Netz jeweils ein Feuerereignis und vier
weitergeleitete Impulse. Bei Gewicht 0 erreichte kein Empfängerneuron seine
Feuerschwelle. Alle 24 Läufe endeten deshalb ohne Datenkanalaktivität und ohne
Abschluss.

### A4 – Wiederholbarkeit

Bei deaktiviertem Lernen blieben alle 24 Wiederholungszeilen inaktiv. Mit den
bestehenden Regeln war der Ablauf bei identischem Seed deterministisch, der
Netzzustand veränderte sich jedoch zwischen den Wiederholungen. In 3 von 24
Zeilen feuerten Datenkontakte; kein Lauf erreichte den Abschlusskontakt.

Die Ausgabewerte ohne Abschluss sind ausschließlich Diagnosedaten und keine
gültigen Antworten.

### A5 – unterschiedliche Ausgangsanbindungen

Im isolierten Modus blieben M1, M2 und M3 vollständig inaktiv. Mit den
vorhandenen Regeln entstanden Datenkanalereignisse in:

| Variante | Läufe mit Datenkanalaktivität | gültige Abschlüsse |
|---|---:|---:|
| M1 | 1 von 24 | 0 |
| M2 | 3 von 24 | 0 |
| M3 | 3 von 24 | 0 |

Die größere Kontaktgruppe erhöht die Wahrscheinlichkeit, zufällige
Netzaktivität zu beobachten. Sie erzeugt aber weder eine Antwortabsicht noch
einen zuverlässigen Abschluss und ist deshalb nicht automatisch besser.

## D. Reproduzierbarkeit und Tests

Der Kerntestbestand wurde von 21 auf 33 Tests erweitert. Geprüft werden:

1. reproduzierbare und überlappungsfreie Kontaktzuordnung,
2. einzelne Ausgangsereignisse,
3. Bytebildung aus acht Kanälen,
4. mehrfaches Feuern eines Kanals,
5. Trennung von Eingabe- und Antwortphase,
6. Verarbeitung des Antwortreizes,
7. Nullbyte gegenüber fehlender Antwort,
8. neuronaler Abschluss,
9. kontrollierter Abbruch,
10. reproduzierbare Antworten,
11. Zuordnung nach Wiederherstellung,
12. unverändertes Verhalten bei deaktivierter Schnittstelle,
13. Ausschluss einer direkten Sensorkopie.

Alle 33 Tests bestehen. Eine zweite vollständige Messung erzeugte für alle
Zustands-, Ereignis- und Ausgabefelder identische Werte. Nur die bewusst
maschinenabhängige Laufzeitspalte wich ab.

## E. Ressourcenverbrauch

Bei identischem isoliertem Netz meldet die Diagnostik:

| Variante | geschätzter Speicher |
|---|---:|
| M1 | 35.792 Byte |
| M2 | 35.888 Byte |
| M3 | 36.016 Byte |

M2 benötigt damit 96 Byte mehr als M1, M3 weitere 128 Byte. Der Zusatzaufwand
entsteht aus den Kontaktlisten. Die Feuer- und Impulsverarbeitung selbst bleibt
unverändert; die Schnittstelle führt pro Feuerereignis lediglich eine lokale
Rollenzuordnung durch.

Zeitmessungen liegen in den Rohdaten, sind wegen ihrer sehr kurzen Dauer und
der geteilten Ausführungsumgebung aber nicht als belastbarer
Performancevergleich zu interpretieren.

## F. Forschungsfolgerungen

### Empirische Antworten auf die Abschlussfragen

1. **Kann EVE technische Ausgangssignale erzeugen?** Ja, die Schnittstelle
   bildet neuronale Feuerereignisse korrekt auf Datenkanäle ab. Mit vorhandenen
   Lernregeln wurden auch im Forschungsversuch einzelne Datenkontakte aktiv.
2. **Kann EVE eine Antwort neuronal abschließen?** Unter den untersuchten
   Bedingungen nein.
3. **Sind die Ergebnisse reproduzierbar?** Ja, bei identischem Zustand,
   Eingabestrom, Seed und Zuordnung.
4. **Was beeinflusst die Ausgabe?** Netz- und Lernzustand, Seed und Anzahl
   technischer Kontakte beeinflussen die beobachtete Datenaktivität. Der
   isolierte Antwortreiz allein reicht nicht aus.
5. **Was fehlt?** Ein kontrollierter neuronaler Explorationsmechanismus und ein
   erlern- oder explorierbarer Abschluss der Antwort.

### Abgrenzung

Die zehn unvollständigen Datenaktivierungen sind kein Nachweis sinnvoller oder
korrekter Antworten. Es wurde keine Zielbewertung, keine Belohnung, kein
Vorhersagefehler und keine neue Lernregel implementiert. Das Stammhirn erzeugt
keine Zufallsbytes und kopiert keine Eingabe.

## G. Empfehlung

Vor rückmeldungsabhängigem Lernen muss EVE zuverlässig unterschiedliche
Handlungen einschließlich eines Abschlusses erzeugen können. Empfohlen wird
daher eine eng begrenzte Folgestufe zur neuronalen Exploration und
Antwortbeendigung. Erst anschließend sollte Forschungsstufe B die lokale
Zuordnung vorausgegangener neuronaler Ereignisse zu einer Rückmeldung
untersuchen.
