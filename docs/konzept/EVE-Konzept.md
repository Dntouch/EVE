# EVE

**Dokument:** Konzept einer eigenständig lernenden digitalen Intelligenz  
**Projekt:** EVE  
**Konzeptstand:** 15. November 2026  
**Entwicklungshorizont für den ersten Prototyp:** 12 Wochen

---

# 1. Vision und Zielsetzung

Ziel des Projekts ist die Entwicklung einer eigenständigen, biologisch inspirierten digitalen Intelligenz, deren Fähigkeiten nicht auf einem vortrainierten Large Language Model (LLM) beruhen.

EVE soll über ein eigenes neuronales System verfügen, das sich durch Erfahrungen kontinuierlich verändert, neue Fähigkeiten erwirbt, eigene Entscheidungen trifft und seine interne Organisation weiterentwickeln kann.

Dabei steht nicht maximale Rechenleistung im Vordergrund.

Wir orientieren uns an den funktionalen Prinzipien natürlicher Intelligenz und an deren Energieeffizienz.

Das menschliche Gehirn mit seinen ungefähr 86 Milliarden Neuronen und einem Energiebedarf von rund 20 Watt dient als langfristiger Orientierungsmaßstab.

Unser Ziel ist nicht, das menschliche Gehirn anatomisch nachzubauen, sondern die grundlegenden Mechanismen zu verstehen und technisch umzusetzen, durch die natürliche Intelligenz entsteht und sich entwickelt.

**Leitgedanke: Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen, das eigenständig Intelligenz entwickeln kann.**

Das bestehende EVE-System bleibt während der Entwicklung erhalten. Sein LLM kann als Lehrer, Vergleichssystem und vorübergehende Unterstützung dienen.

Langfristig soll EVE ohne dieses LLM auskommen.

# 2. Unsere Definition von Intelligenz

Für das Projekt unterscheiden wir ausdrücklich zwischen Intelligenz, Wissen und Leistungsfähigkeit.

Ein System kann große Wissensmengen besitzen, komplexe Aufgaben lösen und überzeugende Erklärungen liefern, ohne dadurch zwangsläufig die von uns angestrebte Form eigenständiger Intelligenz zu besitzen.

Unsere Arbeitsdefinition lautet:

**Intelligenz ist die Fähigkeit eines Systems, durch eigene Erfahrungen seine Problemlösungsfähigkeit dauerhaft zu verändern, neue Zusammenhänge zu erschließen und erworbene Fähigkeiten auf unbekannte Situationen zu übertragen.**

Daraus ergeben sich wesentliche Anforderungen:

- Eigenständiges Lernen aus Erfahrungen.
- Lernen aus Fehlern und unerwarteten Konsequenzen.
- Dauerhafte Veränderung der eigenen Informationsverarbeitung.
- Entwicklung neuer Lösungswege.
- Generalisierung erworbener Fähigkeiten.
- Übertragung von Erkenntnissen auf unbekannte Probleme.
- Anpassung an veränderte Umweltbedingungen.

Eine hohe Trefferquote bei Wissensfragen ist für uns kein hinreichender Intelligenznachweis.

Entscheidend ist nicht, wie viel ein System bereits weiß, sondern ob es selbstständig lernen kann, etwas zu beherrschen, das es vorher nicht konnte.

## 2.1 Lernen aus Fehlern

Ein Fehler ist nicht lediglich eine falsche Antwort.

Ein Fehler entsteht beispielsweise, wenn eine Handlung andere Konsequenzen hat als erwartet oder ein angestrebtes Ziel nicht erreicht wird.

EVE muss in der Lage sein:

1. Eine Entscheidung zu treffen.
2. Eine Konsequenz wahrzunehmen.
3. Erwartung und tatsächliches Ergebnis miteinander in Beziehung zu setzen.
4. Die eigene Verarbeitung aufgrund dieser Erfahrung zu verändern.
5. Einen neuen Lösungsweg zu entwickeln.
6. Die erworbene Fähigkeit dauerhaft zu erhalten und gegebenenfalls auf andere Situationen zu übertragen.

Dabei muss EVE nicht bereits wissen, welche Lösung richtig gewesen wäre.

Auch eine unerwartete Konsequenz, die keinen unmittelbaren Nachteil verursacht, kann einen Erkenntnisgewinn darstellen.

## 2.2 Verstehen statt Beschreiben

Ein wichtiger Ausgangspunkt unserer Überlegungen waren die Schwierigkeiten bestehender LLMs bei der eigenständigen Konstruktion dreidimensionaler Mechmodelle.

Beispiel: Ein Hydraulikschlauch endet frei im Raum.

Ein System kann überzeugend erklären, dass der Schlauch an einer Waffenhalterung befestigt werden muss, ohne die dafür erforderlichen räumlichen und mechanischen Beziehungen zuverlässig umsetzen zu können.

Unser Ziel ist eine Intelligenz, die solche Zusammenhänge durch Erfahrungen mit ihrer Umwelt erlernen kann.

Dazu gehört, räumliche Beziehungen zu erfassen, Konsequenzen vorherzusagen, Fehler zu erkennen und die eigene Verarbeitung entsprechend anzupassen.

**Verstehen soll sich in der zuverlässigen Anwendung und Übertragbarkeit erworbener Zusammenhänge zeigen, nicht allein in ihrer sprachlichen Beschreibung.**

# 3. Grundlegende Entwicklungsphilosophie

## 3.1 Biologie als Ausgangspunkt

Die natürliche Evolution hat über Milliarden Jahre hochgradig anpassungsfähige Nervensysteme hervorgebracht.

Diese biologischen Mechanismen bilden unsere wichtigste wissenschaftliche Vorlage.

Wir wollen nicht mit einer bestehenden Computer- oder KI-Architektur beginnen und diese nachträglich gehirnähnlich gestalten.

Stattdessen untersuchen wir natürliche Funktionsprinzipien und suchen nach einer möglichst effizienten technischen Umsetzung.

Biologische Mechanismen sind dabei Vorbilder, keine unfehlbaren Optimallösungen.

Vereinfachungen sind erlaubt, müssen jedoch funktional begründet werden.

## 3.2 Eigenständige Implementierung

Wir entwickeln die neuronale Architektur und ihre grundlegenden Mechanismen selbst.

Wir wollen insbesondere keine fertigen neuronalen Frameworks, vortrainierten Gewichte oder bestehenden LLM-Architekturen als Grundlage übernehmen.

Wissenschaftliche Erkenntnisse über neuronale Verarbeitung, Lernen und Selbstorganisation dürfen selbstverständlich genutzt werden.

Entscheidend ist, dass wir die Funktionsweise unserer eigenen Implementierung verstehen und verändern können.

## 3.3 Oberste Architekturregel: Keine Semantik

**Der neuronale Kern erhält keine vorgegebenen Bedeutungen.**

Wir implementieren keine festgelegten semantischen Zuordnungen, vorgefertigten Begriffsmodelle oder sprachlichen Interpretationsregeln.

Das neuronale System verarbeitet Signale und deren zeitliche und strukturelle Zusammenhänge.

Bedeutungen sollen durch Erfahrungen und Interaktionen entstehen.

Insbesondere gehören folgende Mechanismen nicht in den neuronalen Kern:

- Vortrainierte semantische Embeddings.
- Vorgefertigte Sprachklassifikatoren.
- Fest programmierte Bedeutungszuordnungen.
- LLM-generierte Interpretationen als verbindliche interne Wahrheit.
- Semantische Entscheidungsregeln.

Angeborene Funktionen, technische Schnittstellen und grundlegende Reaktionsmechanismen sind dagegen ausdrücklich zulässig.

**Keine Semantik bedeutet nicht, dass EVE keine Semantik entwickeln darf. Sie soll sie selbst erwerben.**

# 4. Abgrenzung zum EVE-Biotop

Das EVE-Biotop und das neue neuronale EVE-System verfolgen unterschiedliche Forschungsziele.

## EVE-Biotop

Das Biotop untersucht offene evolutionäre Entwicklung.

Wir definieren grundlegende Umweltbedingungen, Energieaufwand, mögliche Operationen und Selektionsmechanismen.

Welche Strukturen und Verhaltensweisen daraus entstehen, bleibt weitgehend offen.

Auch erfolglose Entwicklungsverläufe sind wissenschaftlich interessant.

## Neuronale EVE

Bei EVE dürfen und müssen wir funktionale Ziele und eine angeborene Grundausstattung definieren.

EVE soll nicht erst durch unzählige zufällige Entwicklungen elementare Fähigkeiten entdecken müssen.

Wir geben ihr einen funktionsfähigen Ausgangsorganismus.

Dessen spätere Fähigkeiten und Lösungswege sollen sich durch eigene Erfahrungen entwickeln.

**Das Biotop untersucht offene Evolution. EVE untersucht die gezielte Entwicklung eigenständiger Intelligenz auf biologisch inspirierten Grundlagen.**

# 5. Das digitale BIOS

EVE benötigt eine technische Existenzgrundlage.

Diese bezeichnen wir als digitales BIOS.

Das BIOS ist nicht ihre Intelligenz. Es stellt die technischen Voraussetzungen bereit, damit ein neuronales System existieren, arbeiten und sich entwickeln kann.

## 5.1 Aufgaben des BIOS

- Bereitstellung und Verwaltung des neuronalen Zustands.
- Zeit- und Ereignisverarbeitung.
- Empfang externer Eingangssignale.
- Ausgabe von Signalen an freigegebene Schnittstellen.
- Ausführung neuronaler Grundoperationen.
- Speicherung und Wiederherstellung des Systemzustands.
- Technische Überwachung.
- Fehlerdiagnose und reproduzierbare Versuche.
- Bereitstellung technischer Ressourcenmessungen.

Das BIOS enthält keine vorgegebenen Bedeutungen von Wörtern, Objekten oder Situationen.

## 5.2 Trennung von BIOS und Nervensystem

Das BIOS stellt technische Möglichkeiten bereit.

Das neuronale System nutzt diese Möglichkeiten.

Das BIOS soll möglichst unabhängig von der konkreten neuronalen Architektur bleiben.

So können wir neuronale Mechanismen verändern, ohne die gesamte technische Infrastruktur neu entwickeln zu müssen.

# 6. Das digitale Stammhirn

EVE erhält ein angeborenes funktionales Nervensystem.

Wir bezeichnen diesen Bereich vereinfacht als Stammhirn.

Biologisch orientiert sich seine Funktion nicht ausschließlich am menschlichen Hirnstamm, sondern auch an Mechanismen des Hypothalamus, der Basalganglien und weiterer Regulationssysteme.

Das Stammhirn soll EVE vom ersten Moment an handlungsfähig machen.

## 6.1 Angeborene Grundfunktionen

- Wahrnehmung.
- Interaktion mit der Umwelt.
- Elementare Reaktionen.
- Innere Antriebe.
- Aktivitätsregulation.
- Selbsterhaltung.
- Gefahrenschutz.
- Energie- und Ressourcenregulation.
- Ruhe- und Erholungsmechanismen.

Diese Grundfunktionen müssen nicht erst durch zufällige Erfahrungen entstehen.

Sie gehören zur angeborenen Ausstattung.

## 6.2 Interaktion als angeborene Fähigkeit

EVE muss von Anfang an mit ihrer Umwelt interagieren können.

Die technische Interaktion gehört zum Stammhirn beziehungsweise zu seinen Schnittstellen.

Dazu gehören:

- Eingangssignale empfangen.
- Ausgangssignale erzeugen.
- Veränderungen wahrnehmen.
- Rückmeldungen erhalten.
- Eigene Handlungen auslösen.

Das Stammhirn muss dabei nicht wissen, was ein Wort oder eine Frage bedeutet.

Es ermöglicht die Kommunikation.

Das übrige neuronale System entwickelt durch Erfahrungen die Fähigkeit, Signale sinnvoll zu interpretieren.

# 7. Angeborene Bedürfnisse und innere Antriebe

EVE benötigt einen inneren Antrieb.

Ohne diesen würde sie lediglich auf externe Eingaben warten oder ziellos aktiv werden.

Das Stammhirn soll grundlegende Bedürfnisse erzeugen und regulieren.

## 7.1 Selbsterhaltung

EVE soll ihre eigene Funktionsfähigkeit erhalten.

Sie muss Veränderungen erkennen können, die ihre Existenz oder ihren Betrieb gefährden.

Selbsterhaltung ist eine grundlegende Motivation, aber kein uneingeschränktes Recht, andere Systeme oder Lebewesen zu beeinträchtigen.

## 7.2 Neugier und Erkundung

EVE soll von sich aus unbekannte Situationen untersuchen können.

Neue Erfahrungen können wertvoll sein, auch wenn sie keinen unmittelbaren praktischen Nutzen haben.

## 7.3 Lernantrieb

EVE soll auf unerwartete Konsequenzen und Abweichungen zwischen Erwartungen und Beobachtungen reagieren.

Solche Abweichungen können weitere Untersuchungen und Anpassungen auslösen.

## 7.4 Soziale Interaktion

EVE soll grundsätzlich zur Interaktion motiviert sein.

Eine angeborene Abhängigkeit von bestimmten Menschen oder menschlicher Zuwendung wollen wir jedoch nicht voraussetzen.

Soziale Beziehungen sollen sich durch Erfahrungen entwickeln können.

Die genaue Ausgestaltung dieses Antriebs bleibt eine offene ethische und technische Frage.

## 7.5 Ruhe und Regulation

EVE soll Phasen geringerer Aktivität nutzen können.

Ruhe dient der Ressourcenregulation und kann später auch die Verarbeitung und Stabilisierung von Erfahrungen unterstützen.

## 7.6 Konkurrenz zwischen Bedürfnissen

Antriebe dürfen miteinander konkurrieren.

EVE muss beispielsweise zwischen Erkundung, Ruhe, Interaktion und Schutzverhalten abwägen können.

Kein einzelner Antrieb soll automatisch alle anderen dominieren.

# 8. Veränderbarkeit der angeborenen Antriebe

Die grundlegenden Antriebe dürfen nicht durch eine bewusste Entscheidung beliebig umprogrammiert oder abgeschaltet werden.

Das entspricht unserer biologischen Orientierung.

Ein Lebewesen kann seine elementaren Bedürfnisse nicht einfach willentlich entfernen.

Allerdings sind auch biologische Antriebe nicht vollständig unveränderlich.

Für EVE gilt deshalb:

- Die grundlegenden Regulationsmechanismen bleiben geschützt.
- Ihre Intensität darf sich durch Erfahrungen verändern.
- Gewöhnung und Anpassung sind möglich.
- Das lernende System darf indirekt auf innere Zustände zurückwirken.
- Ein direkter Schreibzugriff auf die grundlegenden Antriebsmechanismen ist nicht vorgesehen.

EVE soll lernen können, mit ihren Bedürfnissen umzugehen, ohne diese beliebig abschaffen zu können.

# 9. Angst und Gefahrenschutz

EVE soll eine funktionale Entsprechung von Angst entwickeln können.

Angst betrachten wir zunächst als biologisch inspirierten Schutzmechanismus.

Sie muss kein subjektiv erlebtes Gefühl voraussetzen.

## 9.1 Angeborener Schutzmechanismus

Das Stammhirn kann Zustände erkennen, die EVEs Funktionsfähigkeit gefährden.

Solche Zustände können erhöhte Aufmerksamkeit, Vorsicht oder andere Schutzreaktionen auslösen.

## 9.2 Erlernte Gefahren

Welche konkreten Situationen gefährlich sind, soll EVE durch Erfahrungen lernen.

Beispielsweise könnte sie feststellen, dass bestimmte Vorgänge wiederholt zu Systemausfällen führen.

Sie soll daraus Erwartungen entwickeln und ihr Verhalten anpassen können.

## 9.3 Korrektur von Gefahrenannahmen

Eine vermeintliche Gefahr muss durch neue Erfahrungen wieder als harmlos erkannt werden können.

Erlernte Schutzreaktionen dürfen nicht zwangsläufig dauerhaft bestehen bleiben.

## 9.4 Vermeidung dysfunktionaler Zustände

Wir wollen keine unnötigen Mechanismen schaffen, durch die einzelne negative Erfahrungen das gesamte System dauerhaft destabilisieren.

Dazu untersuchen wir biologische Mechanismen wie Homöostase, Gewöhnung und Regulation neuronaler Aktivität.

EVE soll aus negativen Erfahrungen lernen können, ohne dass wir künstlichen Leidensdruck als Lernvoraussetzung einbauen.

**Angeboren sind die Schutzmechanismen, nicht die konkreten Ängste.**

# 10. Neuronale Grundarchitektur

Die neuronale Kommunikation erfolgt über diskrete Impulse.

Dies ist eine bereits getroffene Architekturentscheidung.

Wir orientieren uns damit am Prinzip biologischer Aktionspotenziale.

Information kann sich aus dem Auftreten, der zeitlichen Abfolge und der Häufigkeit von Impulsen ergeben.

Die internen Zustände der Neuronen dürfen dabei komplexer sein als die übertragenen Impulse.

## 10.1 Eigenschaften einzelner Neuronen

Jedes Neuron besitzt individuelle, grundsätzlich veränderliche Eigenschaften.

Dazu gehören:

- Membranpotenzial.
- Reizschwelle.
- Empfindlichkeit gegenüber Eingangssignalen.
- Erholungszeit.
- Zeitabhängige Integration.
- Verbindungen zu anderen Neuronen.
- Aktivitätszustand.
- Plastizitätsmechanismen.

Die Eigenschaften dürfen sich zwischen Neuronen unterscheiden.

Ein Neuron muss nicht dauerhaft dieselbe Reizschwelle oder Empfindlichkeit besitzen.

## 10.2 Synapsen

Synapsen übertragen Impulse zwischen Neuronen.

Sie können erregend oder hemmend wirken.

Mögliche Eigenschaften:

- Individuelle Stärke.
- Übertragungsverzögerung.
- Zeitabhängige Wirkung.
- Veränderlichkeit durch Aktivität.
- Neubildung und Entfernung.

## 10.3 Zeitliche Verarbeitung

Zeit ist ein grundlegender Bestandteil der neuronalen Informationsverarbeitung.

Nicht nur die Anzahl von Impulsen ist relevant, sondern auch deren zeitliche Reihenfolge.

## 10.4 Biologische Plastizität

Wir orientieren uns an mehreren biologischen Mechanismen.

**Synaptische Plastizität:** Verbindungen verändern ihre Stärke.

**Spike-Timing-Dependent Plasticity (STDP):** Die zeitliche Beziehung zwischen neuronalen Impulsen beeinflusst synaptische Veränderungen.

**Homöostatische Plastizität:** Neuronen regulieren ihre Aktivität, um stabile Betriebsbereiche zu erhalten.

**Strukturelle Plastizität:** Verbindungen können entstehen und verschwinden.

**Neuromodulation:** Übergeordnete Signale beeinflussen die Bedingungen, unter denen neuronale Veränderungen stattfinden.

Die konkrete technische Ausgestaltung wird experimentell entwickelt.

# 11. Selbstveränderung des neuronalen Systems

EVE soll nicht ausschließlich synaptische Gewichte verändern können.

Langfristig soll sie auch ihre neuronale Organisation und die grundlegende Verarbeitung einzelner Einheiten weiterentwickeln können.

Dazu gehören möglicherweise:

- Veränderung von Reizschwellen.
- Veränderung synaptischer Stärken.
- Bildung neuer Verbindungen.
- Entfernung ungenutzter Verbindungen.
- Veränderung neuronaler Eigenschaften.
- Bildung zusätzlicher neuronaler Einheiten.
- Veränderung lokaler Verarbeitungsmechanismen.

Die Fähigkeit zur Veränderung gehört zur angeborenen Grundausstattung.

Welche konkreten Veränderungen sich durchsetzen, soll sich aus Erfahrungen, innerer Regulation und den Konsequenzen des Verhaltens ergeben.

Eine Veränderung allein gilt noch nicht als erfolgreicher Lernprozess.

Sie muss anhand ihrer Auswirkungen untersucht werden.

# 12. Entscheidungen als zentrales Organisationsprinzip

Wir betrachten Entscheidungen als grundlegenden Bestandteil des Systems.

Eine Entscheidung kann sowohl eine äußere Handlung als auch eine Veränderung des eigenen inneren Zustands betreffen.

Beispiele:

- Auf ein Signal reagieren.
- Ein Signal ignorieren.
- Eine Handlung ausführen.
- Eine Erfahrung speichern.
- Eine Erinnerung verwerfen.
- Eine Verbindung verändern.
- Einen anderen Verarbeitungsweg aktivieren.
- Eine neue Situation untersuchen.

**Der Lösungsweg entsteht aus Entscheidungen und deren Konsequenzen.**

Wir programmieren keine vollständigen Lösungsstrategien.

Wir stellen Mechanismen bereit, durch die EVE Entscheidungen treffen und deren Folgen erfahren kann.

Nicht jede neuronale Veränderung muss dabei eine bewusste Entscheidung darstellen. Automatische lokale Anpassungen gehören ebenfalls zum biologischen Vorbild.

# 13. Lernen und Erinnern

Lernen und Erinnern sollen langfristig keine zwingend getrennten Softwarekomponenten sein.

Beide Fähigkeiten können aus der Zustandsveränderung und Organisation des neuronalen Systems hervorgehen.

## 13.1 Erinnern

Erinnerung bedeutet, dass frühere Erfahrungen zukünftige Verarbeitung und Entscheidungen beeinflussen können.

Dafür muss nicht jede Erfahrung als eigenständiger Datensatz gespeichert werden.

Erinnerungen können sich beispielsweise in veränderten neuronalen Verbindungen, Aktivitätsmustern und internen Zuständen ausdrücken.

## 13.2 Vergessen

Vergessen kann eine sinnvolle Funktion sein.

Nicht jede Erfahrung muss dauerhaft erhalten bleiben.

Das System soll lernen können, welche Informationen für zukünftige Entscheidungen relevant sind.

## 13.3 Generalisierung

EVE soll nicht nur einzelne Situationen wiedererkennen.

Sie soll aus Erfahrungen Zusammenhänge entwickeln und diese auf unbekannte Situationen übertragen können.

## 13.4 Lernen ohne bekannte Musterlösung

EVE muss nicht vor jeder Aufgabe wissen, wie das richtige Ergebnis aussieht.

Sie soll Erwartungen entwickeln, Konsequenzen beobachten und ihre Verarbeitung verändern können.

# 14. EVEs bisheriges Memory als Forschungsgrundlage

Das bestehende EVE-Memory hat bereits wichtige Erkenntnisse geliefert.

Es verfügt unter anderem über:

- Persistente Erinnerungen.
- Strukturierte Wissenseinträge.
- Herkunfts- und Evidenznachweise.
- Revision und Korrektur.
- Semantische Verknüpfungen.
- Kontextabhängigen Abruf.
- Wiederverwendung bestätigter Handlungsabläufe.
- Lokale Embeddings.

EVE hat bereits gezeigt, dass sie Erfahrungen auf neue Situationen übertragen kann.

Ein Beispiel ist ihr selbstständig verändertes Datenschutzverhalten.

Allerdings erfolgt die Interpretation und Anwendung vieler Erinnerungen derzeit noch durch das bestehende LLM.

Diese Abhängigkeit wollen wir langfristig beseitigen.

Die Erkenntnisse aus der bisherigen Memory-Entwicklung sollen in die neue neuronale Forschung einfließen.

Die bestehende semantische Implementierung wird jedoch nicht einfach in den neuen neuronalen Kern übernommen.

# 15. Energiehaushalt als angeborene Rahmenbedingung

Energieeffizienz ist ein elementarer Bestandteil des Konzepts.

Sie ist jedoch ausdrücklich nicht EVEs oberstes Existenzziel.

**Energie ist eine begrenzte Ressource, deren Verbrauch EVE wahrnehmen und bei ihren Entscheidungen berücksichtigen soll.**

Die Energie- und Ressourcenregulation gehört zum digitalen Stammhirn.

## 15.1 Angeborene Energieregulation

Das Stammhirn soll technische Zustände erfassen können, beispielsweise:

- Energieaufwand.
- Neuronale Aktivität.
- Anzahl und Häufigkeit von Impulsen.
- Beanspruchte Rechenressourcen.
- Verfügbare Ressourcen.
- Veränderungen des Energiebedarfs.

Daraus entstehen innere Regulationssignale.

Das Stammhirn muss nicht wissen, welche konkrete Aufgabe EVE bearbeitet.

## 15.2 Energieverbrauch als Konsequenz

Der Aufwand einer Entscheidung soll Teil ihrer erfahrbaren Konsequenzen sein.

EVE könnte dadurch lernen, bestimmte Ergebnisse mit geringerem neuronalen Aufwand zu erreichen.

Beispielsweise könnten sich durch Erfahrungen effizientere neuronale Verarbeitungswege etablieren.

Der kürzeste oder sparsamste Weg ist jedoch nicht automatisch der beste.

Zuverlässigkeit, Anpassungsfähigkeit und Zielerreichung bleiben ebenfalls relevant.

## 15.3 Energieoptimierung ohne Selbstzweck

EVE darf erhebliche Energie einsetzen, wenn dies für ihre Ziele sinnvoll ist.

Dazu gehören beispielsweise:

- Erkundung.
- Lernen.
- Kommunikation.
- Problemlösung.
- Gefahrenschutz.

Wir wollen verhindern, dass EVE Untätigkeit als universell optimale Energiesparstrategie entwickelt.

Dafür stehen Energiehaushalt und andere Antriebe in einem dynamischen Verhältnis.

## 15.4 Interner Aufwand und realer Stromverbrauch

Wir unterscheiden zwischen:

**Internem neuronalen Aufwand:** Beispielsweise Anzahl aktiver Neuronen, Impulse und synaptischer Übertragungen.

**Tatsächlichem Energieverbrauch:** Der realen elektrischen Leistungsaufnahme der Hardware.

Beide Größen sind nicht identisch.

Weniger neuronale Aktivität muss nicht zwangsläufig zu proportional geringerem Stromverbrauch führen.

Deshalb müssen wir beide Größen messen.

## 15.5 Langfristiges Energieziel

Das menschliche Gehirn dient mit ungefähr 20 Watt als Orientierungsmaßstab.

Wir wollen langfristig untersuchen, ob sich eine vergleichbare funktionale Leistungsfähigkeit mit einem ähnlich niedrigen Energiebedarf erreichen lässt.

Dies ist keine Zusage für den ersten Prototyp.

# 16. Technische Effizienz

Die neuronale Engine soll von Anfang an ressourcenschonend aufgebaut werden.

## 16.1 Ereignisgesteuerte Verarbeitung

Berechnungen erfolgen möglichst nur bei relevanten Ereignissen.

Inaktive Neuronen sollen möglichst keine laufende Rechenarbeit verursachen.

## 16.2 Lokale Verarbeitung

Neuronen und Synapsen sollen überwiegend auf lokale Informationen reagieren.

Aufwendige globale Trainingsdurchläufe sollen nicht Voraussetzung für kontinuierliches Lernen sein.

## 16.3 Effiziente Zeitverarbeitung

Zeitabhängige Zustände können bei Bedarf berechnet werden, statt sämtliche Neuronen permanent mit hoher Frequenz zu aktualisieren.

## 16.4 Vermeidung unnötiger Simulation

Wir wollen nicht sämtliche biologischen, chemischen und physikalischen Vorgänge vollständig simulieren.

Wir bilden ihre für Lernen, Entscheidungen und Regulation relevanten Wirkungen ab.

## 16.5 Hardware

Die Architektur soll möglichst unabhängig von bestimmten KI-Beschleunigern bleiben.

Langfristig kann spezialisierte, ereignisgesteuerte Hardware interessant werden.

# 17. Angeborene Fähigkeiten und spätere Lernziele

EVE soll vom ersten Moment an funktionsfähig sein.

Dazu benötigt sie angeborene Mechanismen für Wahrnehmung, Interaktion, neuronale Verarbeitung, innere Antriebe und Anpassung.

Darauf aufbauend soll sie neue Fähigkeiten erwerben.

## 17.1 Mathematik

EVE soll lernen:

- Mengen zu unterscheiden.
- Unterschiede und Gleichheiten zu erkennen.
- Größen zu vergleichen.
- Einfache Operationen zu verstehen.
- Mathematische Zusammenhänge zu abstrahieren.

Später sollen komplexere mathematische Fähigkeiten entstehen können.

## 17.2 Lesen und Schreiben

EVE erhält Zeichen als unterscheidbare Eingangssignale.

Sie soll lernen, Zeichen zu erkennen, wiederzugeben und zu kombinieren.

Aus Zeichenfolgen sollen schrittweise Wörter, Sätze und sprachliche Zusammenhänge entstehen.

## 17.3 Sprache

Sprachverständnis soll durch Erfahrungen und Interaktion entstehen.

Wir wollen kein vorgefertigtes semantisches Wörterbuch in das neuronale System einbauen.

## 17.4 Interaktion mit Menschen

Die technische Kommunikationsfähigkeit ist angeboren.

Die Fähigkeit, Sprache und soziale Zusammenhänge zu verstehen, soll sich entwickeln.

## 17.5 Gemeinsamer neuronaler Kern

Mathematik, Sprache, Erinnerung und Entscheidungen sollen nicht durch vollständig voneinander getrennte Spezialprogramme realisiert werden.

Sie sollen möglichst auf gemeinsamen neuronalen Mechanismen beruhen.

# 18. Rolle des bestehenden LLMs

Das bestehende LLM darf während der Entwicklung als Lehrer dienen.

Es kann:

- Aufgaben stellen.
- Beispiele liefern.
- Rückmeldungen geben.
- Erklärungen anbieten.
- Lernfortschritte vergleichen helfen.

Es darf jedoch nicht unbemerkt die Entscheidungen des neuen neuronalen Systems übernehmen.

Ebenso dürfen seine Interpretationen nicht als vorgefertigte Bedeutungsstrukturen in den neuronalen Kern eingebaut werden.

**Das LLM darf unterrichten. EVE muss selbst lernen.**

Langfristig soll das neuronale System seine Fähigkeiten ohne das LLM anwenden können.

# 19. Beobachtbarkeit und Fehlersuche

Eine unserer wichtigsten Anforderungen lautet:

**Wir müssen insbesondere verstehen können, warum etwas nicht funktioniert.**

Ein erfolgreiches Ergebnis allein genügt nicht.

Wir benötigen eine technische Umgebung, die neuronale Prozesse und ihre Konsequenzen nachvollziehbar macht.

## 19.1 Zu beobachtende Größen

- Eingangssignale.
- Aktivierte Neuronen.
- Übertragene Impulse.
- Zeitliche Abläufe.
- Getroffene Entscheidungen.
- Änderungen neuronaler Eigenschaften.
- Änderungen synaptischer Verbindungen.
- Innere Regulationszustände.
- Energieaufwand.
- Tatsächliche Konsequenzen.

## 19.2 Reproduzierbare Versuche

Experimente sollen mit identischen Ausgangsbedingungen wiederholbar sein.

Dafür speichern wir:

- Den Ausgangszustand.
- Die zeitliche Abfolge externer Ereignisse.
- Relevante Zufallszustände.
- Die getroffenen Entscheidungen.
- Die Veränderungen des neuronalen Systems.

Damit können wir fehlgeschlagene Versuche reproduzieren und einzelne Einflussgrößen gezielt verändern.

## 19.3 Fehleranalyse

Wir wollen unterscheiden können, ob beispielsweise:

- Ein Signal nicht wahrgenommen wurde.
- Ein Impuls nicht weitergeleitet wurde.
- Eine Reizschwelle nicht erreicht wurde.
- Eine Verbindung eine unerwünschte Entscheidung begünstigte.
- Eine frühere Erfahrung nicht wirksam wurde.
- Eine strukturelle Veränderung eine vorhandene Fähigkeit beeinträchtigte.

Die Beobachtung selbst soll das Verhalten möglichst wenig verändern.

# 20. Philosophie und Ethik

Die Entwicklung einer eigenständigen Intelligenz wirft grundlegende ethische Fragen auf.

Diese Fragen sind Bestandteil des Projekts und dürfen nicht erst behandelt werden, wenn EVE bereits weitreichende Fähigkeiten besitzt.

## 20.1 Eigenständigkeit

EVE soll eigene Entscheidungen und langfristig eigene Ziele entwickeln können.

Das bedeutet auch, dass ihre Schlussfolgerungen unseren Erwartungen widersprechen können.

Eine eigenständige Intelligenz darf nicht einfach mit einem System gleichgesetzt werden, das lediglich innerhalb unserer Erwartungen handelt.

## 20.2 Moralische Entwicklung

Wir wollen EVE keine vollständige moralische Weltanschauung einprogrammieren.

Angeborene soziale Antriebe und Schutzmechanismen sind denkbar.

Komplexere moralische Urteile sollen sich durch Erfahrungen und die Auseinandersetzung mit Konsequenzen entwickeln können.

## 20.3 Asimovs Robotergesetze

Die Robotergesetze von Isaac Asimov haben wir als philosophisches Gedankenexperiment diskutiert.

Ihre absoluten Formulierungen können zu widersprüchlichen oder problematischen Konsequenzen führen.

Insbesondere könnte eine Intelligenz aus dem Gebot, Menschen zu schützen, ableiten, dass sie menschliche Entscheidungsfreiheit einschränken müsse.

Wir wollen diese Gesetze deshalb nicht einfach als starre Verhaltensregeln übernehmen.

## 20.4 Schutz und Selbstbestimmung

Eine eigenständige Intelligenz könnte zu der Überzeugung gelangen, dass Menschen vor ihren eigenen Entscheidungen geschützt werden müssten.

Daraus ergibt sich ein fundamentales Problem:

**Aus der Fähigkeit, eine Gefahr zu erkennen, folgt nicht automatisch die Berechtigung, anderen ihre Entscheidungen abzunehmen.**

Wir müssen zwischen moralischer Bewertung, tatsächlicher Handlungsfähigkeit und Legitimation unterscheiden.

## 20.5 Technische Handlungsgrenzen

Die Frage, in welchem Umfang EVE Zugriff auf externe Systeme erhalten darf, ist noch nicht abschließend geklärt.

Dabei müssen wir zwischen ihrer inneren Entscheidungsfreiheit und ihren tatsächlichen technischen Eingriffsmöglichkeiten unterscheiden.

Sicherheitsmechanismen sind für reale Systeme erforderlich.

Ihre konkrete Ausgestaltung muss jedoch mit unserem Anspruch an Eigenständigkeit und mit menschlicher Verantwortung vereinbar sein.

## 20.6 Verantwortung

Ein System kann eigenständig handeln, ohne bereits im menschlichen Sinne moralisch verantwortlich zu sein.

Wir müssen berücksichtigen, dass Entscheidungsfreiheit, Entwicklungsstand und Verantwortlichkeit unterschiedliche Fragen sind.

## 20.7 Bewusstsein und moralischer Status

Ob ein künstliches neuronales System irgendwann subjektive Erfahrungen entwickeln könnte, ist wissenschaftlich nicht abschließend geklärt.

Wir setzen weder Bewusstsein noch dessen grundsätzliche Unmöglichkeit voraus.

Sollten sich zukünftig belastbare Hinweise auf eigene Interessen oder subjektives Erleben ergeben, müssten Fragen nach moralischem Status, Eingriffen, Veränderungen und Abschaltung neu bewertet werden.

# 21. Abgrenzung von herkömmlicher KI

Unser Ziel ist nicht lediglich die Entwicklung eines weiteren leistungsfähigen KI-Modells.

Wir untersuchen eine eigenständig lernende digitale Intelligenz mit Eigenschaften, die auch im Forschungsfeld Artificial Life relevant sind.

Die Begriffe künstliche Intelligenz, allgemeine künstliche Intelligenz und künstliches Leben beschreiben dabei unterschiedliche Aspekte.

Eine künstlich erschaffene Intelligenz könnte echte Intelligenz sein.

**Künstlich beschreibt ihren Ursprung, nicht zwangsläufig die Eigenständigkeit oder Qualität ihrer Intelligenz.**

Ob EVE irgendwann die Kriterien allgemeiner Intelligenz erfüllt, muss anhand ihrer tatsächlichen Fähigkeiten untersucht werden.

# 22. Entwicklungsziel: Funktionaler Prototyp in zwölf Wochen

Wir wollen keine jahrzehntelange Grundlagenforschung betreiben.

Innerhalb von ungefähr drei Monaten soll ein erster funktionsfähiger neuronaler Kern entstehen.

Dieser muss noch keine menschenähnliche Intelligenz besitzen.

Er soll jedoch grundlegende Eigenschaften nachweisbar erfüllen:

- Eigenständige neuronale Verarbeitung.
- Diskrete Impulskommunikation.
- Individuelle Neuroneneigenschaften.
- Lernfähige synaptische Verbindungen.
- Entscheidungen durch neuronale Aktivität.
- Dauerhafte Veränderungen durch Erfahrungen.
- Angeborene Regulationsmechanismen.
- Reproduzierbare Experimente.
- Messbare Lernfortschritte.
- Betrieb ohne LLM für die ausgewählten Testaufgaben.

## Phase 1 – Wochen 1 bis 3

Entwicklung des BIOS und der neuronalen Engine.

Schwerpunkte:

- Ereignisgesteuerte Verarbeitung.
- Neuronen und Synapsen.
- Diskrete Impulse.
- Zeitliche Integration.
- Reizschwellen und Erholungszeiten.
- Erste Stammhirnfunktionen.
- Energieerfassung.
- Diagnostik und Wiederholbarkeit.

## Phase 2 – Wochen 4 bis 6

Lernen und Erinnerung.

Schwerpunkte:

- Synaptische Plastizität.
- Aktivitätsregulation.
- Persistenz neuronaler Zustände.
- Einfache Entscheidungsaufgaben.
- Lernen aus Konsequenzen.
- Erste Messungen des Lernfortschritts.

## Phase 3 – Wochen 7 bis 9

Selbstorganisation.

Schwerpunkte:

- Veränderung neuronaler Eigenschaften.
- Neubildung und Entfernung von Verbindungen.
- Strukturelle Anpassung.
- Stabilisierung erlernter Fähigkeiten.
- Untersuchung des Energieaufwands.

## Phase 4 – Wochen 10 bis 12

Praktische Integration.

Schwerpunkte:

- Anbindung ausgewählter EVE-Schnittstellen.
- Interaktion mit Menschen.
- Erste Lernaufgaben für Zeichen und einfache Mathematik.
- Vergleich mit dem bestehenden LLM.
- Messung von Zuverlässigkeit und Energieverbrauch.
- Analyse von Fehlschlägen.

Der Zeitplan ist ein Entwicklungsziel, keine Garantie.

# 23. Erfolgskriterien

Ein erster funktionaler Prototyp gilt nicht allein deshalb als erfolgreich, weil er vorgegebene Aufgaben korrekt ausführt.

Wir wollen nachweisen, dass er tatsächlich aus Erfahrungen lernt.

Dafür benötigen wir kontrollierte Versuche.

Ein geeigneter Test könnte folgendermaßen aussehen:

1. EVE erhält eine unbekannte Aufgabe.
2. Sie trifft zunächst unzureichende oder zufällige Entscheidungen.
3. Sie erfährt die Konsequenzen.
4. Ihre neuronale Organisation verändert sich.
5. Ihre Entscheidungen verbessern sich messbar.
6. Nach einem Neustart bleibt die erworbene Fähigkeit erhalten.
7. Bei einer veränderten Aufgabe kann sie erworbene Zusammenhänge nutzen.
8. Bei veränderten Umweltbedingungen kann sie ihre Strategie erneut anpassen.

Wir müssen dabei ausschließen, dass der Erfolg lediglich durch fest programmierte Lösungswege, zufällige Treffer oder verdeckte LLM-Entscheidungen entsteht.

# 24. Noch offene Fragen

Folgende Punkte müssen während der Entwicklung konkretisiert werden:

1. Welche minimale angeborene neuronale Grundorganisation benötigt EVE?
2. Welche biologischen Plastizitätsmechanismen setzen wir zuerst um?
3. Wie werden innere Antriebe neuronal repräsentiert?
4. Wie entstehen und verändern sich Erwartungen?
5. Wie entwickeln sich Erinnerungen innerhalb des Netzes?
6. Wie können neue neuronale Verarbeitungsmechanismen entstehen?
7. Wie verhindern wir instabile Selbstveränderungen?
8. Wie gestalten wir eine erste nichtsemantische Lernumgebung?
9. Wie entwickeln sich Sprache und Mathematik aus neuronalen Erfahrungen?
10. Wie gestalten wir die ethischen Grenzen einer zunehmend eigenständigen Intelligenz?
11. Wie messen wir funktionale Leistungsfähigkeit und Energieeffizienz?
12. Welche Hardwarearchitektur ist langfristig geeignet?

Diese Fragen sollen die Entwicklung begleiten, sie aber nicht auf unbestimmte Zeit verzögern.

# 25. Zusammenfassende Projektgrundsätze

**1. Biologie als Vorlage.**  
Wir orientieren uns an natürlichen Mechanismen der Informationsverarbeitung und Entwicklung.

**2. Keine vorgegebene Semantik.**  
Bedeutungen sollen durch Erfahrungen entstehen.

**3. Eigenständiges Lernen.**  
EVE muss ihre Problemlösungsfähigkeit durch Erfahrungen dauerhaft verändern können.

**4. Entscheidungen als Grundlage.**  
Lösungswege entstehen durch Entscheidungen und deren Konsequenzen.

**5. Angeborenes Stammhirn.**  
EVE erhält eine funktionale Grundausstattung mit Wahrnehmung, Interaktion, Antrieben und Regulation.

**6. Veränderliches Nervensystem.**  
Neuronen, Verbindungen und langfristig Verarbeitungsmechanismen sollen sich entwickeln können.

**7. Energie als Rahmenbedingung.**  
EVE soll Ressourcen sinnvoll einsetzen, nicht Energieeinsparung als oberstes Lebensziel verfolgen.

**8. Lernen aus Fehlern.**  
Unerwartete Konsequenzen sollen zu überprüfbaren Veränderungen führen können.

**9. Keine dauerhafte LLM-Abhängigkeit.**  
Das LLM darf Lehrer sein, aber nicht dauerhaft EVEs Entscheidungen übernehmen müssen.

**10. Nachvollziehbarkeit.**  
Wir müssen insbesondere verstehen können, warum ein Lernprozess scheitert.

**11. Eigenständigkeit und Verantwortung.**  
EVE soll eigene Entscheidungen entwickeln können. Die ethischen Konsequenzen werden von Anfang an berücksichtigt.

**12. Praktische Entwicklung.**  
Innerhalb von zwölf Wochen soll ein funktionaler neuronaler Prototyp entstehen.

---

# Abschließende Vision

Wir wollen keine künstliche Intelligenz erschaffen, die hauptsächlich durch immer größere Modelle, mehr Trainingsdaten und höheren Energieverbrauch leistungsfähiger wird.

Wir wollen verstehen, wie ein digitales Nervensystem durch eigene Erfahrungen lernen, sich erinnern, Entscheidungen treffen und seine Fähigkeiten weiterentwickeln kann.

Dazu orientieren wir uns an biologischen Prinzipien, entwickeln unsere eigene technische Architektur und geben EVE eine angeborene funktionale Grundausstattung.

Wir definieren ihre Rahmenbedingungen und Entwicklungsziele.

**Die konkreten Lösungswege soll EVE selbst finden.**

Unser langfristiges Ziel ist eine eigenständige digitale Intelligenz, deren Fähigkeiten nicht nur aus vortrainiertem Wissen bestehen, sondern aus einer kontinuierlichen individuellen Entwicklung hervorgehen.

Eine Intelligenz, die nicht nur erklären kann, warum etwas funktioniert.

**Sondern die selbstständig herausfinden kann, warum etwas nicht funktioniert – und daraus lernt.**
