# EVE – Forschungsstufe A
## Neuronale Ausgabe und eigenständige Antwortbildung

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Grundlage:** Neuronales Grundkonzept, Experimente 001–003 und das Konzept zur angeborenen Lernfähigkeit des Stammhirns  
**Status:** Forschungs- und Entwicklungsauftrag

---

## 1. Zielsetzung

Erweitere EVE um eine motorische Ausgabeschnittstelle, über die das neuronale Netzwerk eigenständig binäre Informationen erzeugen kann.

Bisher kann EVE technische Eingaben empfangen und neuronal verarbeiten. Eine eigenständige Ausgabe, die aus neuronaler Aktivität entsteht, ist noch nicht ausreichend definiert.

Forschungsstufe A soll diese Lücke schließen.

Zentrale Forschungsfrage:

**Kann EVEs neuronales Netzwerk aus empfangenen Reizen heraus eigenständige, beobachtbare und reproduzierbare Ausgaben erzeugen, ohne dass das Stammhirn die Antwort vorgibt?**

Es geht noch nicht darum, dass diese Ausgaben korrekt sind.

Zunächst muss die technische Grundlage dafür entstehen, dass EVE überhaupt selbstständig antworten kann.

## 2. Architekturprinzip

Die bestehende Aufgabenteilung bleibt erhalten.

### BIOS

Das BIOS übernimmt:

- Empfang technischer Eingaben.
- Bereitstellung von Eingabe- und Ausgabeschnittstellen.
- Verwaltung abgeschlossener Ein- und Ausgabevorgänge.
- Technische Protokollierung.
- Persistenz und Wiederherstellung.

### Stammhirn

Das Stammhirn übernimmt:

- Umwandlung eingehender Rohdaten in neuronale Reize.
- Bereitstellung motorischer Ausgabekanäle.
- Erfassung neuronaler Ausgangsaktivität.
- Kennzeichnung von Eingabe-, Antwort- und Abschlussphasen.
- Bereitstellung technischer Signale zur Aufforderung einer Antwort.

Das Stammhirn darf keine Antwort aus der ursprünglichen Eingabe berechnen oder übernehmen.

### Neuronales Netzwerk

Das neuronale Netzwerk:

- Empfängt die sensorischen Reize.
- Verarbeitet diese mithilfe seiner bestehenden Neuronen und Synapsen.
- Erzeugt eigene Feuerereignisse.
- Kann darüber Ausgangskanäle aktivieren.

Eine ausgegebene Information muss auf neuronale Aktivität zurückzuführen sein.

## 3. Motorische Ausgabekanäle

Ergänze acht technische Ausgangskanäle.

Diese entsprechen den acht Bitpositionen eines Bytes.

Ein Ausgangskanal kann durch neuronale Feuerereignisse aktiviert werden.

Für die erste Version gilt:

- Ein während der Antwortphase aktivierter Ausgangskanal liefert Bit 1.
- Ein nicht aktivierter Kanal liefert Bit 0.
- Mehrere Feuerereignisse desselben Kanals innerhalb einer Antwort verändern seinen Bitwert nicht zusätzlich.
- Die acht Kanäle bilden zusammen einen möglichen Bytewert.

Die Kanäle definieren ausschließlich die technische Ausgabe.

Sie besitzen keine vorgegebene semantische Bedeutung.

Das Netzwerk weiß nicht, welche Zeichen, Wörter oder anderen Informationen damit dargestellt werden.

## 4. Anbindung an das neuronale Netzwerk

Die motorischen Ausgangskanäle werden an vorhandene Neuronen angebunden.

Für jeden Kanal wird eine reproduzierbar ausgewählte Gruppe von Neuronen als technischer Ausgangskontakt verwendet.

Feuert ein angeschlossenes Neuron während der Antwortphase, aktiviert es den entsprechenden Ausgangskanal.

Diese Anbindung darf nicht das reguläre Feuern oder die synaptische Verarbeitung verändern.

### Experimentelle Varianten

Untersuche zunächst:

| Variante | Neuronen pro Ausgangskanal |
|---|---:|
| M1 | 1 |
| M2 | 4 |
| M3 | 8 |

Für acht Kanäle ergeben sich damit 8, 32 beziehungsweise 64 Ausgangskontakte.

Die Zuordnung muss:

- Deterministisch reproduzierbar sein.
- Dokumentiert werden.
- Ohne semantische Vorbelegung erfolgen.
- Zwischen Vergleichsläufen konstant bleiben.
- Keine zusätzlichen neuronalen Lernregeln benötigen.

Ein Neuron darf weiterhin normale eingehende und ausgehende Synapsen besitzen.

Die technischen Ausgangskontakte stellen lediglich eine Beobachtungs- beziehungsweise Ausgabemöglichkeit bereit.

Für den ersten Versuch sollen die Kontaktgruppen verschiedener Ausgangskanäle nicht überlappen.

## 5. Antwortabschluss und gültige Ausgaben

Ein entscheidendes Problem besteht darin, zwischen einer tatsächlichen Antwort und fehlender Aktivität zu unterscheiden.

Wenn kein Ausgangskanal feuert, ergibt sich technisch das Bitmuster `00000000`.

Das darf jedoch nicht automatisch als bewusst erzeugte Antwort gewertet werden.

Andernfalls könnte ein vollständig inaktives Netzwerk scheinbar erfolgreich ein Nullbyte ausgeben.

### Vorgabe

Ergänze deshalb ein separates technisches Abschlusssignal.

Dieses Signal muss ebenfalls durch neuronale Aktivität ausgelöst werden können.

Dafür kann ein eigener motorischer Abschlusskontakt verwendet werden.

Er wird genauso wie die acht Datenkanäle reproduzierbar an vorhandene Neuronen angebunden.

Die Auswertung unterscheidet anschließend:

- **Gültige Antwort:** Neuronales Abschlusssignal empfangen.
- **Keine Antwort:** Kein Abschlusssignal empfangen.
- **Unvollständige Antwort:** Verarbeitung musste vor Abschluss abgebrochen werden.

Eine gültig abgeschlossene Antwort darf auch `00000000` enthalten.

Das Stammhirn darf eine fehlende Antwort nicht eigenmächtig durch ein Nullbyte ersetzen.

Die beobachteten Datenkanäle sollen auch bei einer nicht abgeschlossenen Antwort für Diagnosezwecke protokolliert werden.

## 6. Verarbeitung einzelner Lernversuche

Jeder Versuch besteht aus klar voneinander getrennten Phasen.

### Phase 1 – Eingabe

Das BIOS übergibt eine Bit- beziehungsweise Bytefolge.

Das Stammhirn wandelt die Information mithilfe der bestehenden sensorischen Schnittstelle in neuronale Impulse um.

Die neuronale Ereignisverarbeitung läuft regulär.

### Phase 2 – Abschluss der Eingabe

Die Eingabesequenz wird technisch abgeschlossen.

Ausstehende neuronale Ereignisse werden gemäß der vorhandenen Ereignisverarbeitung abgearbeitet.

Das Ende der Eingabe ist nicht automatisch der Beginn einer gültigen Antwort.

### Phase 3 – Antwortaufforderung

Das Stammhirn stellt einen technischen Antwortreiz bereit.

Dieser Reiz enthält:

- Keine richtige Antwort.
- Keine Zielfolge.
- Keine Hinweise auf den erwarteten Bytewert.

Er signalisiert lediglich, dass eine Ausgabe angefordert wird.

Dafür kann ein separater sensorischer Steuerkanal eingerichtet werden.

Die konkrete technische Anbindung muss reproduzierbar und unabhängig von den acht Datenkanälen sein.

### Phase 4 – Neuronale Antwort

EVE verarbeitet den Antwortreiz.

Während dieser Phase wird die Aktivität der motorischen Ausgangskontakte erfasst.

Die Antwort entsteht ausschließlich aus den beobachteten neuronalen Feuerereignissen.

### Phase 5 – Abschluss

Der Versuch endet durch:

- Das neuronale Abschlusssignal.
- Das vollständige Abklingen der verarbeitbaren Aktivität ohne Abschluss.
- Das Erreichen eines konfigurierbaren Sicherheitslimits.

Die technischen Ereignisgrenzen dürfen vom BIOS beziehungsweise Stammhirn verwaltet werden.

Es werden keine synaptischen Zeitstempel benötigt.

## 7. Keine globale neuronale Uhr

Die bisherige ereignisgesteuerte Architektur bleibt erhalten.

Insbesondere gelten:

- Keine globalen neuronalen Ticks.
- Keine regelmäßige Aktualisierung sämtlicher Neuronen.
- Keine Zeitstempel pro Synapse.
- Keine permanenten globalen Aktivitätsscans.

Die Ausgabeschnittstelle reagiert auf tatsächlich auftretende Feuerereignisse.

Für die Abgrenzung einzelner Versuche dürfen technische Zustände im BIOS beziehungsweise Stammhirn verwendet werden.

Diese dienen ausschließlich der Steuerung des Versuchsablaufs.

## 8. Untersuchungsvarianten

### Versuch A1 – Spontane Ausgangsaktivität

Initialisiere das Netzwerk und untersuche zunächst ohne gezielten Eingabereiz:

- Entsteht überhaupt neuronale Ausgangsaktivität?
- Welche motorischen Kanäle werden aktiviert?
- Entstehen vollständige Antworten?
- Gibt es Unterschiede zwischen den Ausgangskontaktvarianten?

Falls keine Aktivität entsteht, dokumentiere dieses Ergebnis.

Es dürfen keine künstlichen Antworten erzeugt werden.

### Versuch A2 – Reaktion auf sensorische Eingaben

Präsentiere einfache Eingaben:

- `00`
- `01`
- `10`
- `11`
- `A`
- `B`
- `AB`
- `BA`

Untersuche die nachfolgende neuronale Antwort.

Dabei ist ausdrücklich zwischen einer durch den Reiz ausgelösten Aktivität und einer eigenständig erzeugten Antwort zu unterscheiden.

### Versuch A3 – Antwortaufforderung

Wiederhole die Eingabeversuche mit einem separaten Antwortreiz.

Vergleiche:

- Verhalten ohne Antwortaufforderung.
- Verhalten mit Antwortaufforderung.
- Aktivität der motorischen Kanäle.
- Häufigkeit gültiger Abschlüsse.
- Unterschiede der ausgegebenen Bitmuster.

### Versuch A4 – Wiederholbarkeit

Präsentiere dieselben Eingaben mehrfach.

Untersuche:

- Entstehen identische Ausgaben?
- Wie stark unterscheiden sich Antworten?
- Welche neuronalen Zustände beeinflussen die Ausgabe?
- Sind die Ergebnisse bei identischem Ausgangszustand reproduzierbar?

### Versuch A5 – Unterschiedliche Ausgangsanbindungen

Vergleiche die Varianten M1, M2 und M3.

Untersuche insbesondere:

- Anzahl gültiger Antworten.
- Aktivierte Ausgangskanäle.
- Häufigkeit vollständig gesetzter oder vollständig inaktiver Bitmuster.
- Antwortvielfalt.
- Reproduzierbarkeit.
- Zusätzlichen Speicher- und Rechenaufwand.

Eine größere Ausgangsgruppe darf nicht automatisch als bessere neuronale Ausgabe interpretiert werden.

## 9. Lernregeln und Ausgangskonfiguration

Forschungsstufe A untersucht zunächst die Möglichkeit zur Ausgabe.

Es werden keine neuen Rückmeldungs- oder Belohnungsregeln eingeführt.

Für die technische Grundprüfung sind Lernen und Wachstum zunächst deaktiviert, damit die Wirkung der Ausgabeschnittstelle isoliert untersucht werden kann.

Zusätzlich dürfen gesondert gekennzeichnete Versuche mit den vorhandenen Lern- und Wachstumsregeln durchgeführt werden.

Alle dabei verwendeten Regulationsparameter müssen angegeben werden.

Die Ergebnisse der beiden Betriebsarten sind getrennt auszuwerten.

## 10. Exploratives Verhalten

Im neuen Stammhirnkonzept wurde festgelegt, dass EVE später auch ohne vorhandene Lösungsstrategie unterschiedliche Handlungen ausprobieren können soll.

Die konkrete Form dieser Exploration ist noch offen.

Für Forschungsstufe A gilt:

- Keine fertigen Ausgabeantworten.
- Keine direkte Weiterleitung der Eingabedaten.
- Keine automatische Ausgabe zufällig erzeugter Bytes durch das Stammhirn.
- Keine gezielte Erzeugung korrekter Ergebnisse.

Sollte das Netzwerk keine gültigen Antworten erzeugen, ist dies ein zulässiges Forschungsergebnis.

Ein eigenständiger Explorationsmechanismus wird erst in einer anschließenden Forschungsstufe definiert und überprüft.

## 11. Diagnose und Beobachtung

Ergänze eine technische Beobachtung der Ausgangsverarbeitung.

Erfasse mindestens:

- Versuch und Zufallsstartwert.
- Eingabedaten.
- Verwendete Ausgangsanbindung.
- Anzahl neuronaler Feuerereignisse.
- Aktivierte motorische Kanäle.
- Reihenfolge motorischer Aktivierungen.
- Erzeugtes Bitmuster.
- Anzahl gültiger Antwortabschlüsse.
- Versuche ohne Antwort.
- Abgebrochene Versuche.
- Verarbeitete synaptische Ereignisse.
- Speicherverbrauch.
- Rechenzeit.

Die Messinstrumentierung darf keine Veränderungen am neuronalen Verhalten auslösen.

Für Detailanalysen muss nachvollziehbar sein, welche Neuronen die technischen Ausgänge aktiviert haben.

## 12. Reproduzierbarkeit und Persistenz

Die Ausgangsanbindungen und die technischen Zustände eines laufenden Versuchs müssen eindeutig dokumentiert werden.

Bei identischem Netzwerkzustand, identischer Konfiguration, identischem Seed und identischem Eingabestrom müssen die Ergebnisse reproduzierbar sein.

Falls ein Versuch während einer aktiven Antwortphase gespeichert wird, muss entweder eine vollständige Wiederherstellung möglich sein oder eine solche Speicherung ausdrücklich ausgeschlossen werden.

Diese Entscheidung ist technisch zu dokumentieren.

## 13. Automatisierte Tests

Ergänze Tests für mindestens:

1. Reproduzierbare Zuordnung motorischer Kontakte.
2. Korrekte Verarbeitung einzelner Ausgangsereignisse.
3. Bildung eines Bytes aus acht Ausgangskanälen.
4. Mehrfaches Feuern desselben Ausgangskanals.
5. Trennung von Eingabe- und Antwortphase.
6. Korrekte Verarbeitung eines Antwortreizes.
7. Unterscheidung zwischen Nullbyte und fehlender Antwort.
8. Korrekte Verarbeitung des neuronalen Abschlusssignals.
9. Kontrollierten Abbruch bei Ereignislimit.
10. Reproduzierbare Ausgaben.
11. Wiederherstellung gespeicherter Zustände.
12. Unverändertes Verhalten des neuronalen Kerns bei deaktivierter Ausgabeschnittstelle.
13. Ausschluss einer direkten Eingabe-Ausgabe-Kopie.

Die bestehenden automatisierten Tests müssen weiterhin bestehen.

## 14. Nicht Bestandteil dieser Forschungsstufe

Folgende Funktionen werden ausdrücklich noch nicht implementiert:

- Ergebnisbewertung durch das Stammhirn.
- Belohnungs- und Bestrafungssignale.
- Vorhersagefehler.
- Erwartungsbildung.
- Rückmeldungsabhängige Gewichtsänderungen.
- Lokales Ereignisgedächtnis als neue Lernregel.
- Eligibility Traces.
- Neue Erregungs- oder Hemmungsmechanismen.
- Synaptische Abschwächung.
- Sprachverständnis.
- Vorgegebene Zeichen- oder Worterkennung.
- Community-Lernen.

Diese Bestandteile folgen in späteren, getrennt untersuchten Forschungsstufen.

## 15. Forschungsbericht

Erstelle einen vollständigen Forschungsbericht mit folgenden Schwerpunkten:

### A. Technische Umsetzung

Wie wurden die motorischen Schnittstellen implementiert?

### B. Neuronale Ausgabe

Unter welchen Bedingungen entstehen tatsächlich Ausgangsereignisse?

### C. Antwortverhalten

Welche Unterschiede bestehen zwischen spontaner Aktivität, reizausgelöster Aktivität und explizit angeforderten Antworten?

### D. Ausgangsanbindung

Welche Auswirkungen haben unterschiedliche Kontaktgruppengrößen?

### E. Reproduzierbarkeit

Sind die beobachteten Ergebnisse wiederholbar?

### F. Ressourcenverbrauch

Welchen zusätzlichen Rechen- und Speicheraufwand verursacht die Ausgabeschnittstelle?

### G. Forschungsfolgerungen

Welche technischen und neuronalen Voraussetzungen fehlen gegebenenfalls noch, damit EVE zuverlässig eigenständige Antworten erzeugen kann?

## 16. Lieferumfang

Erstelle:

- Eine dokumentierte motorische Ausgabeschnittstelle.
- Eine reproduzierbare Versuchsumgebung.
- Automatisierte Tests.
- Maschinenlesbare Rohdaten.
- Einen vollständigen Forschungsbericht als Markdown.
- Eine Entscheidungsvorlage für die anschließende Forschungsstufe.

Die bestehende neuronale Architektur muss erhalten bleiben.

Änderungen an vorhandenen Lernmechanismen sind nicht zulässig.

## 17. Abschlusskriterium

Forschungsstufe A ist abgeschlossen, wenn wir empirisch beantworten können:

1. Kann EVE aus ihrer neuronalen Aktivität technische Ausgangssignale erzeugen?
2. Kann sie eine Antwort neuronal abschließen?
3. Sind die Ergebnisse reproduzierbar?
4. Welche Eingaben und neuronalen Zustände beeinflussen die Ausgabe?
5. Welche zusätzlichen Mechanismen benötigen wir für eigenständige Exploration und anschließendes Lernen?

**Eine korrekte Antwort auf ein vorgegebenes Muster ist für Forschungsstufe A ausdrücklich noch nicht erforderlich.**

---

## Zentrale Forschungsfrage

**Kann EVE eigenständig Informationen aus ihrem neuronalen Netzwerk nach außen übertragen, ohne dass das Stammhirn den Inhalt dieser Informationen vorgibt?**

Mit dieser Fähigkeit schaffen wir die technische Voraussetzung für den späteren Lernkreislauf:

Wahrnehmung → neuronale Verarbeitung → Handlung → Umweltbeobachtung → Rückmeldung → Lernen.
