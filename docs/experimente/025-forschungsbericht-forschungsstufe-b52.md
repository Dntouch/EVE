# Forschungsbericht – Forschungsstufe B.5.2

## A. Ergebnis in einem Satz

Der größte Teil der untersuchten Fähigkeitsverluste ist keine synaptische
Löschung, sondern eine reproduzierbare transiente Abrufstörung; sensorische,
inhibitorische und gemeinsame Kandidaten tragen zusätzlich bei, rechtfertigen
aber noch keinen einzelnen neuen lokalen Gedächtnismechanismus.

## B. Referenz und Umfang

B.5.1 wird im getrennten Reproduktionsordner unverändert ausgeführt. B.5.2
untersucht dieselben 768 Netze und 393.216 Episoden. 509 bestehende und 16
neue Tests bestehen. Entdeckung und unbenutzte Bestätigung, drei
Stromfamilien, normale und inverse Rollen, E1/E2, Aufgabenmatrix,
Kontrollgruppen und 64 Persistenzläufe sind vollständig enthalten.

## C. Synaptische Speicherung und funktionaler Abruf

Von 15.194 detaillierten Verlusten werden 14.731 durch Wiederherstellung des
früheren vollständigen transienten Zustands bei unveränderten neuen Gewichten
wieder korrekt. 463 sind durch die synaptischen Änderungen hinreichend
erklärt. Insbesondere bleiben bei 12.452 Verlusten Gewichte und Struktur
vollständig gleich. Eine nicht abrufbare Bindung ist damit in der Mehrzahl
der untersuchten Zustände keine gelöschte Bindung.

Der transiente Faktor allein reproduziert 14.636 Verluste, sensorische
Änderung allein 188 und inhibitorische Änderung allein 275. Weitere 296
Verluste entstehen erst durch Faktorinteraktionen. Die Ergebnisse trennen
notwendige, hinreichende und beitragende Ursachen, statt Ereigniskategorien
als Kausalität zu behandeln.

## D. Eligibility, gemeinsame Kandidaten und Probeeffekt

Von 149.301 inhibitorischen Änderungen sind 48.867 zeitlich berechtigt, aber
in der bewerteten Episode weder funktional aktiv noch durch Einzelablation
für die autonome Ausgabe nachweisbar. Pauschales Zurückrollen verbessert die
gleichzeitige Leistung nach 16 Erfahrungen jedoch nur von 157 auf 159 von
768 Netzen. Eine präzisere lokale Kreditierung bleibt plausibel, ist aber als
Produktionsregel nicht ausreichend begründet.

In 80.588 Episoden sind Kandidaten beider Reize vorhanden und teilen einen
Empfänger. Sensorische Interferenz ist somit real, aber verglichen mit dem
transienten Anteil nur selten allein hinreichend.

Fingerprint-identische Kopien liefern ausnahmslos dieselbe Probe. Wiederholte
Proben auf derselben Runtime stimmen dagegen nur in 267/15.194 Fällen überein.
Die Diagnose verändert den zu diagnostizierenden Zustand. Ereignisfreie
Zeitfortschreibung bis 64 Zeiteinheiten stellt die Antwort nicht automatisch
wieder her.

## E. Gleichzeitige Beziehungen

Alle 768 Netze erreichen beide Beziehungen gleichzeitig. Nach dem ersten
gemeinsamen Erfolg bleiben nach einer Erfahrung 531/768, nach acht 203/768
und nach 64 nur 141/768 gleichzeitig korrekt. Auch späterer oder mindestens
acht Episoden stabiler gemeinsamer Erfolg schützt nicht dauerhaft. Die
allgemeinen E1-Ziele und 48/64 robusten Seeds werden in keiner
Entdeckungs- oder Bestätigungskombination erreicht.

## F. Hypothesen

- H1 ist stark bestätigt: transienter Zustand ist der dominante hinreichende
  Faktor der detaillierten Verluste.
- H2 ist als häufige nichtnotwendige inhibitorische Kandidatur bestätigt,
  aber nicht als alleiniger Produktionsengpass.
- H3 ist bestätigt: X und Y besitzen gemeinsame Empfänger und Kandidaten.
- H4 ist bestätigt: 296 untersuchte Verluste erfordern Faktorinteraktionen.
- H5 ist bestätigt: synaptische Speicherung bleibt häufig vorhanden, während
  Konkurrenz den funktionalen Abruf verhindert.
- H6 ist bestätigt: sequenzielle Proben verändern den Runtime-Zustand und
  dürfen nicht als unabhängige Fähigkeitsmessungen behandelt werden.

## G. Antworten A bis J

1. **A:** 463/15.194 detaillierte Verluste sind durch die neuen synaptischen
   Zustände hinreichend; sensorische und inhibitorische Einzelanteile sind
   188 und 275.
2. **B:** 14.731/15.194 Antworten werden mit alten transienten Zuständen bei
   neuen Gewichten wiederhergestellt; 12.452 Gesamtverluste besitzen gar
   keine Gewichtsänderung.
3. **C:** Eingangs-, lokale Ereignisspeicher-, Membran- und
   Synapsenaktivitätszustände unterscheiden die Verluste. Der technisch
   gültige vollständige Runtime-Zustand ist hinreichend; einzelne private
   Bestandteile sind noch nicht unabhängig transplantierbar.
4. **D:** Inhibitorische Änderung allein erzeugt 275 Detailverluste und kann
   spätere Ausgaben verändern; pauschaler Rollback stabilisiert beide
   Beziehungen jedoch nicht.
5. **E:** Ja. 48.867 Änderungen sind zeitlich berechtigt, aber in der Episode
   weder funktional aktiv noch durch Einzelablation notwendig.
6. **F:** In 80.588 Episoden teilen Kandidaten beider Reize mindestens einen
   Empfänger und vermitteln mögliche Kreuzinterferenz.
7. **G:** 296 Detailverluste treten erst durch Kombination mehrerer Faktoren
   auf.
8. **H:** Ja. 14.731 ausgewählte Antworten werden ohne Gewichtsänderung durch
   den früheren vollständigen transienten Zustand wiederhergestellt.
9. **I:** Nicht robust. Nach 64 Erfahrungen sind vom ersten gemeinsamen
   Erfolg nur 141/768 Netze weiterhin für beide Beziehungen korrekt.
10. **J:** Noch nein. Der Befund rechtfertigt weitere lokale Zerlegung des
    transienten Zustands, aber weder Reset noch Gewichtsschutz oder pauschale
    Eligibility-Unterdrückung als neue Funktion.

## H. Abschlussentscheidung

B.5.2 ist als Ursachenanalyse erfolgreich. K9/K10 bleiben deaktiviert. B.5.3
soll vor jeder Funktionsänderung die minimalen, lokal beobachtbaren
Bestandteile des transienten Runtime-Zustands und ihre Abhängigkeit vom
Probeimpuls isolieren. Eine Normalisierung darf nur diagnostisch geprüft
werden und muss die andere Beziehung sowie Umweltumkehr erhalten.
