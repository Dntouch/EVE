# Technische Kausalanalyse B.5.10 – einzelne Schreibereignisse

## Ergebnis

B.5.10 kann eine einzelne konkrete Gewichtsoperation innerhalb des normalen
Modulationswegs beobachten und ausschließlich diese Operation auslassen. Die
Eligibility wird trotzdem verbraucht; weitere Kandidaten desselben Kontakts,
Ereigniswarteschlange, Simulationszeit und Zufallszustand bleiben bis zum
Eingriff identisch. Der Eingriff ist eine standardmäßig leere
Forschungs-Schnittstelle und keine Lernregel.

## Schreibpfad

`Runtime::apply_local_modulation` berechnet für jeden Eligibility-Kandidaten
zuerst Gewicht, Delta und begrenztes Zielgewicht. Direkt vor der Mutation
entsteht eine `PlasticityObservation` mit:

- Ereignisindex und Simulationszeit,
- Modulationssynapse, Sender, Empfänger und Kandidat,
- erregender oder hemmender Wirkung,
- Gewicht vor der Operation, vorgeschlagenem Gewicht und Delta,
- Modulations- und lokaler Polarität,
- Eligibility-Generation, Entstehungs-/Ablaufzeit und Kandidatenmenge,
- Membran und lokalem Aktivitätszähler,
- auslösendem Mechanismus.

Der ältere aktivitätsabhängige Verstärkungspfad ist in der Referenz weiterhin
deaktiviert. Alle primären B.5.10-Schreibungen stammen aus der bestehenden
Eligibility-Modulation. Bei einer inhibitorischen Synapse kann positive
Gewichtsänderung durch die beitragsbewusst invertierte lokale Polarität
entstehen.

## Diagnostische Intervention

Der neue `PlasticityWriteInterceptor` erhält genau diese Vorher-Beobachtung.
Nur wenn er für die konkrete Kombination aus Kandidat, Eligibility-Generation,
Vorher- und Zielgewicht `true` liefert, wird diese Operation als
`isolated_write_suppressed` markiert. Danach läuft die Kandidatenschleife
normal weiter. Der Eligibility-Datensatz wird an derselben Stelle wie in der
Referenz verbraucht.

Die Varianten sind:

- W0: unveränderter natürlicher Schreibkontakt,
- W1: genau eine vorab identifizierte Operation ausgelassen,
- W2: unbeeinflusste Kontrollkopie,
- W3: vorab festgelegte Kombination konkreter Operationen ausgelassen,
- W4: vorhandene globale Diagnosesperre.

Für W0 bis W4 wird nach der neuronalen Aktivitätsphase und unmittelbar vor dem
Modulationskontakt derselbe vollständige Runtime-Zustand kopiert. Der erste
beobachtete Schreibkandidat bestätigt Kandidat, Generation, Vorhergewicht und
Zielgewicht erneut am tatsächlichen Mutationspunkt. W0 und W2 waren in allen
119 prospektiven Fällen nach dem Kontakt fingerabdruckgleich. W1 unterdrückte
in allen Fällen exakt eine Operation. Alle 595 W0–W4-Kopien waren gültig.

## Natürliche Fortsetzung

Nach dem Eingriff werden nur die äußeren Reiz- und initialen
Explorationspläne gepaart. Jede Kopie erzeugt ihre eigene Motorantwort und
erhält daraus ihre tatsächliche Rückmeldung. Eine divergierte Handlung wird
nicht mit der Rückmeldung des Referenzlaufs überschrieben. Gemessen wird nach
1, 4, 8, 16, 32, 64 und 128 weiteren Erfahrungen.

## Historische Ereignisse

Die B.5.9-Reproduktion bestätigt 52 gegenüber dem ersten gemeinsamen Erwerb
erhöhte Gewichte und zehn einzeln rücknehmbare Verbindungen. Auf diesen zehn
Verbindungen liegen 13 konkrete positive Schreibereignisse. Wird jeweils nur
dieses Ereignis an seinem Kontakt ausgelassen und die Kopie anschließend
natürlich bis zum historischen Verlust fortgeführt, bleiben beide Beziehungen
in 13/13 Fällen erhalten.

Zwei Verbindungen enthalten je zwei Verstärkungsschritte. In diesen Fällen
ist jeder Schritt im beobachteten Pfad notwendig, weil bereits das Auslassen
eines Schritts das spätere kritische Gewicht verhindert. Die kombinierte
Unterdrückung der bis zum Verlust erhöhten Verbindungen erhält die gemeinsame
Fähigkeit in 12/12 historischen Fällen. Das ist eine externe historische
Diagnose und kein online verfügbarer Gewichtsschutz.

## Prospektive Ereignisse

Discovery und Bestätigung verwenden neue 64er-Seedbereiche 70001–70064 und
80001–80064 für drei Stromfamilien und beide Zuordnungen, insgesamt je 384
Netze. Die detaillierte Ereignisstichprobe wurde vor der Folgewirkung über fünf
Regeln ausgewählt: erste erregende, erste inhibitorische, weitere positive,
grenznahe und konkurrierende Änderung. Je Stratum wurden die ersten fünf Seeds
untersucht; 119 von 120 vorgesehenen Erwerbs-/Umkehrfällen lieferten ein
passendes Ereignis.

Die Stichprobe enthält 77 erregende und 42 inhibitorische Änderungen. Von 119
Einzelereignissen waren 21 unmittelbar notwendig, 98 zunächst neutral. Nach
128 natürlichen Erfahrungen blieben 84 neutral, drei wurden verzögert nützlich
und elf verzögert schädlich. Eine unmittelbar schädliche Änderung trat in
dieser prospektiven Stichprobe nicht auf. Elf notwendige Ereignisse stammen
aus Erwerb, zehn aus Umweltumkehr.

## Informationsverfügbarkeit

Direkt lokal vorhanden (A) sind Gewicht, Wirkung, Empfänger, Membran,
Refraktärzustand, lokaler Aktivitätszähler und Eligibility. Aus der aktuellen
lokalen Integration berechenbar (B) sind Marge sowie erregende und hemmende
Beiträge. Vollständige Beitrags-, Margen-, Konkurrenz- und Schreibfolgen sind
nur im Forschungsprotokoll rekonstruiert (C). Die funktionale Folgewirkung ist
erst nach der Erfahrung bekannt (D).

Die bestätigte Ereignisklassifikation erreichte L0 53/59 und L4 51/59. Die
extern rekonstruierten Historien H1 bis H4 erzeugten viele unbekannte
Signaturen und verschlechterten die Bestätigung auf 34/59, 28/59, 22/59 und
22/59. Diese kleine, stark neutral dominierte Stichprobe ist kein Nachweis
eines sicheren Selektors. Insbesondere stehen H1–H4 in der geprüften Form
nicht als persistente lokale Erinnerung zur Verfügung.

## Persistenz und Ressourcen

Der Interceptor wird weder gespeichert noch gehasht und ist nach Erzeugung
einer Runtime leer. Persistenzformat 15 bleibt unverändert. Laden und
deterministische Fortsetzung waren identisch; der geprüfte Zustand umfasste
5.683 Byte. Es entstehen keine zusätzlichen persistenten Bytes je Neuron oder
Synapse.

Der vollständige Lauf benötigte auf der Messmaschine rund 35,0 CPU-Sekunden,
20.672 KiB Spitzen-RSS und 501 diagnostische Runtime-Kopien. Die Rohdaten
umfassen rund 24,53 MB und liegen deterministisch komprimiert mit rund 1,46 MB
Speicherbedarf vor. Maschinenabhängig sind nur Zeit und RSS.

## Technische Bewertung

Die Schnittstelle erfüllt die Kausaldiagnose und verändert bei leerem
Interceptor das Verhalten nicht. Sie darf integriert werden. Eine
automatische Schreibauswahl, ein Rollback, ein Stabilitäts-Gate oder eine neue
persistente Historie folgen daraus nicht.
