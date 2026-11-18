# 0008 – Übergang zu B.2: zeitlich begrenzte lokale Lernberechtigung

**Status:** Entwurf – gemeinsame Entscheidung ausstehend

**Datum:** 2026-11-18

**Verantwortung:** offen

## Kontext

B.1 hat die in Entscheidungsvorlage 0007 geforderte Wiederholungsstabilität
untersucht. Die bisherige R0-Regel blockiert fortgeführte Handlungen. R1 stellt
Erregbarkeit und Datenaktivität wieder her, verliert in der breiten Stichprobe
aber einzelne M3-Abschlüsse. R2F mit vollständiger einmaliger
Membranentspannung reproduziert die wiederhergestellte `AB`-Referenz über acht
Phasen und über 128 Phasen der etablierten Seeds.

G3-Ereignisgedächtnisse, Synapsen und Lernzähler bleiben durch Erholung
unverändert. Damit sind zwei Voraussetzungen gleichzeitig erfüllt:

1. wiederholt ausführbare neuronale Handlungen,
2. lokale Informationen über vorausgegangene Eingänge.

Noch nicht vorhanden ist eine neuronale, zeitlich begrenzte Markierung, die
eine spätere Rückmeldung an eine konkrete Feuerentscheidung binden kann.

## Entscheidungskriterien

- Eine Lernberechtigung muss neuronaler Zustand sein, nicht Ergebnis der
  technischen M3-Diagnose.
- R2F darf nur kurzfristigen Membranzustand verändern.
- G3-Ring und Eligibility-Zustand müssen getrennt konfigurier- und
  abschaltbar sein.
- Positive und negative Rückmeldung dürfen noch nicht mit Erwartungsbildung
  verwechselt werden.
- R0, R1, R2F und wiederhergestellte Zustände bleiben Kontrollen.
- Zufällige Abschlüsse und technische Abbrüche dürfen keine positive
  Lernberechtigung erhalten.

## Optionen

### Option A – B.2 auf R2F/T3 aufbauen

R2F mit Erholungsdauer 16 wird als Arbeitskonfiguration verwendet. B.2
untersucht zunächst nur Erzeugung, Lebensdauer und Verfall einer lokalen
Eligibility-Markierung. Gewichtsänderungen folgen erst nach eigener
Zwischenentscheidung.

Vorteil ist die höchste beobachtete Wiederholungsstabilität. Nachteil ist, dass
T3 eine technische und keine biologische Konstante bleibt.

### Option B – B.2 parallel mit R1 und R2F durchführen

Jeder Eligibility-Versuch wird vollständig unter R1 und R2F wiederholt.

Dies trennt den Einfluss der Membranentspannung besser, verdoppelt aber die
Versuchsmatrix. R1 bleibt ohnehin als Ablationskontrolle sinnvoll.

### Option C – Vor B.2 ausschließlich die Zeitwarteschlange optimieren

Die sortierte Sequenz wird durch Heap oder Kalenderwarteschlange ersetzt und
bis eine Million Neuronen tatsächlich vermessen.

Das senkt ein technisches Skalierungsrisiko, liefert aber zunächst keine neue
Lernerkenntnis. Bei den untersuchten aktiven Populationen bestand kein
funktionaler Engpass.

### Option D – Direkte Gewichtsänderung aus dem G3-Ring

Alle beim Erfolg noch sichtbaren Synapsen werden sofort verändert.

Diese Option bleibt abzulehnen: B hat nicht beitragende und mehrdeutige
Ringeinträge nachgewiesen. B.1 ändert diesen Befund nicht.

## Vorgeschlagene Entscheidung

Option A mit verpflichtender R1-Ablationskontrolle aus Option B wird empfohlen.

R2F/T3 dient als vorläufige Arbeitskonfiguration. B.2 soll in zwei getrennten
Schritten vorgehen:

1. lokale Eligibility-Markierung ohne Gewichtsänderung,
2. erst nach Auswertung eine gesondert freigegebene rückmeldungsabhängige
   Gewichtsänderung.

## Verbindliche Kontrollen für B.2

- K0: kein G3, keine Eligibility, keine neue Lernwirkung.
- K1: G3, aber Eligibility deaktiviert.
- K2: Eligibility aktiv, Rückmeldungswirkung deaktiviert.
- R1 und R2F mit identischen Seeds, Zeitabständen und Rückmeldungen.
- M3/K2-Unterbrechungen und B-K4-Einzelunterbrechungen.
- Rückmeldung vor, bei und nach Ablauf der Eligibility-Dauer.
- Synapsen mit Gewicht 0 als Nichtbeitragskontrolle.
- getrennte Auswertung von Abschluss ohne Daten, nach Daten und nachgewiesenem
  M3-Weg.
- fortgeführte Sequenzen mindestens bis 128 Phasen.

## Architekturvorbehalte

- R2F setzt nur Membranen tatsächlich erholender Neuronen zurück. Nicht
  feuernde Neuronen behalten ihren Zustand; R2F ist deshalb bewusst keine
  globale Zustandswiederherstellung.
- Die Dauer 16 ist abstrakt. B.2 muss Rückmeldungsabstände relativ zur Dauer
  angeben.
- Die aktuelle Ereigniswarteschlange kann beim Einfügen linear wachsen. Vor
  sehr großen gleichzeitig aktiven Populationen ist eine alternative
  Warteschlange zu prüfen.
- Die technische Diagnose darf Eligibility niemals direkt setzen.

## Konsequenzen

### Positiv

- B.2 baut auf wiederholbaren kausalen Handlungen statt Einzelaktionen auf.
- Lokale Erinnerung überlebt die Erholung.
- Membran-, Gedächtnis- und Synapsenzustand bleiben konzeptionell getrennt.

### Negativ oder begrenzend

- Eligibility erhöht erneut Zustand und Persistenzbedarf.
- R2F ist eine diskrete technische Entspannung, noch kein biologisches Leck.
- Rückmeldungsbedeutung und Erwartungsbildung bleiben ungelöst.

## Nachweise

- [Entwicklungsauftrag B.1](../experimente/008-entwicklungsauftrag-forschungsstufe-b1.md)
- [Forschungsbericht B.1](../experimente/008-forschungsbericht-forschungsstufe-b1.md)
- [Zeit- und Erholungsspezifikation](../konzept/EVE-Ereignisgesteuerte-Zeit-und-neuronale-Erholung.md)
- `research/results/research-stage-b1-timing.csv`
- `research/results/research-stage-b1-repetitions.csv`
- `research/results/research-stage-b1-integrity.csv`
- `research/results/research-stage-b1-feedback.csv`

## Überprüfung

Diese Vorlage ist vor Implementierung einer Eligibility-Markierung gemeinsam
zu entscheiden. Jede tatsächliche Gewichtsänderung benötigt danach eine
weitere ausdrückliche Freigabe.
