plugins {
    `cpp-library`
    `maven-publish`
    `visual-studio`
}

import org.gradle.language.cpp.internal.DefaultCppBinary

group = "at.o2xfs"
version = "1.0-SNAPSHOT"

repositories {
    if((version as String).endsWith("-SNAPSHOT")) {
        maven {
            url = uri("https://repo.fagschlunger.co.at/libs-snapshot-local")
        }
    }
    maven {
        url = uri("https://repo.fagschlunger.co.at/libs-release-local")
    }
}

library {
    linkage.set(listOf(Linkage.SHARED))
    targetMachines.set(listOf(machines.windows.x86, machines.windows.x86_64))

    dependencies {
        implementation("at.o2xfs:o2xfs-common-bin:1.0-SNAPSHOT") {
            attributes {
                attribute(Attribute.of("org.gradle.native.linkage", Linkage::class.java), Linkage.STATIC)
            }
        }
        implementation("eu.cen:cen-xfs-api:3.30")
        implementation("eu.cen:cen-xfs-supp:3.30")
    }
}

tasks.withType(CppCompile::class.java).configureEach {
    macros.put("_UNICODE", null)
    macros.put("UNICODE", null)

    val javaHome = System.getenv("JAVA_HOME") ?: System.getProperty("java.home")
    includes.from(files("$javaHome/include", "$javaHome/include/win32"))
}

tasks.withType(LinkSharedLibrary::class.java).configureEach {
    linkerArgs.addAll("/MD", "user32.lib")
}

tasks.register<Jar>("dist") {
    dependsOn("assembleReleaseX86", "assembleReleaseX86-64")
    archiveFileName.set("o2xfs-xfs-std-bin.jar")
    destinationDirectory.set(file("$buildDir/dist"))
    from("$buildDir/lib/main/release/x86/o2xfs-xfs-std-bin.dll") {
        into("x86")
    }
    from("$buildDir/lib/main/release/x86-64/o2xfs-xfs-std-bin.dll") {
        into("x86_64")
    }
    rename("(.+)-bin(.+)", "$1$2")
}

tasks.withType<PublishToMavenLocal>().configureEach {
    onlyIf {
        publication == publishing.publications["mavenJava"]
    }
}

publishing {
    publications {
        create<MavenPublication>("mavenJava") {
            artifact(tasks["dist"])
        }
    }

    repositories {
        maven {
            url = if((version as String).endsWith("-SNAPSHOT")) uri("https://repo.fagschlunger.co.at/libs-snapshot-local") else uri("https://repo.fagschlunger.co.at/libs-release-local")
            credentials {
                val maven_username: String? by project
                val maven_password: String? by project
                username = maven_username
                password = maven_password
            }
        }
    }
}

