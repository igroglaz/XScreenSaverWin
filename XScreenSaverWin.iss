; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B7914227-CD94-4B6F-859C-E3823C9552DA}
AppName=XScreenSaver for Windows
AppVerName=XScreenSaver v0.52
AppPublisher=Katayama Hirofumi MZ
AppPublisherURL=http://katahiromz.web.fc2.com/xscreensaverwin/eindex.html
AppSupportURL=http://katahiromz.web.fc2.com/xscreensaverwin/eindex.html
AppUpdatesURL=http://katahiromz.web.fc2.com/xscreensaverwin/eindex.html
DefaultDirName={pf}\XScreenSaver for Windows
DefaultGroupName=XScreenSaver for Windows
AllowNoIcons=yes
OutputBaseFilename=xscreensaver_setup
Compression=lzma
SolidCompression=yes
UninstallDisplayIcon={app}\xscreensaver.exe
LicenseFile=LISENCE.txt

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "XScreenSaverWin\xscreensaver.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "XScreenSaverWin\abstractile_scr\abstractile.scr"; DestDir: "{app}\abstractile_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\anemone_scr\anemone.scr"; DestDir: "{app}\anemone_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\anemotaxis_scr\anemotaxis.scr"; DestDir: "{app}\anemotaxis_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\antinspect_scr\antinspect.scr"; DestDir: "{app}\antinspect_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\antmaze_scr\antmaze.scr"; DestDir: "{app}\antmaze_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\antspotlight_scr\antspotlight.scr"; DestDir: "{app}\antspotlight_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\ant_scr\ant.scr"; DestDir: "{app}\ant_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\apollonian_scr\apollonian.scr"; DestDir: "{app}\apollonian_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\atlantis_scr\atlantis.scr"; DestDir: "{app}\atlantis_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\attraction_scr\attraction.scr"; DestDir: "{app}\attraction_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\atunnel_scr\atunnel.scr"; DestDir: "{app}\atunnel_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\blaster_scr\blaster.scr"; DestDir: "{app}\blaster_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\blinkbox_scr\blinkbox.scr"; DestDir: "{app}\blinkbox_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\blocktube_scr\blocktube.scr"; DestDir: "{app}\blocktube_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\boing_scr\boing.scr"; DestDir: "{app}\boing_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\bouboule_scr\bouboule.scr"; DestDir: "{app}\bouboule_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\bouncingcow_scr\bouncingcow.scr"; DestDir: "{app}\bouncingcow_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\boxed_scr\boxed.scr"; DestDir: "{app}\boxed_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\boxfit_scr\boxfit.scr"; DestDir: "{app}\boxfit_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\braid_scr\braid.scr"; DestDir: "{app}\braid_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\bubble3d_scr\bubble3d.scr"; DestDir: "{app}\bubble3d_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\bubbles_scr\bubbles.scr"; DestDir: "{app}\bubbles_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\bumps_scr\bumps.scr"; DestDir: "{app}\bumps_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cage_scr\cage.scr"; DestDir: "{app}\cage_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\ccurve_scr\ccurve.scr"; DestDir: "{app}\ccurve_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\celtic_scr\celtic.scr"; DestDir: "{app}\celtic_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cloudlife_scr\cloudlife.scr"; DestDir: "{app}\cloudlife_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\companion_scr\companion.scr"; DestDir: "{app}\companion_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\compass_scr\compass.scr"; DestDir: "{app}\compass_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\coral_scr\coral.scr"; DestDir: "{app}\coral_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\crackberg_scr\crackberg.scr"; DestDir: "{app}\crackberg_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\critical_scr\critical.scr"; DestDir: "{app}\critical_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\crystal_scr\crystal.scr"; DestDir: "{app}\crystal_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cube21_scr\cube21.scr"; DestDir: "{app}\cube21_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cubenetic_scr\cubenetic.scr"; DestDir: "{app}\cubenetic_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cubestorm_scr\cubestorm.scr"; DestDir: "{app}\cubestorm_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cubicgrid_scr\cubicgrid.scr"; DestDir: "{app}\cubicgrid_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cwaves_scr\cwaves.scr"; DestDir: "{app}\cwaves_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\cynosure_scr\cynosure.scr"; DestDir: "{app}\cynosure_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\dangerball_scr\dangerball.scr"; DestDir: "{app}\dangerball_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\deco_scr\deco.scr"; DestDir: "{app}\deco_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\deluxe_scr\deluxe.scr"; DestDir: "{app}\deluxe_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\demon_scr\demon.scr"; DestDir: "{app}\demon_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\discrete_scr\discrete.scr"; DestDir: "{app}\discrete_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\distort_scr\distort.scr"; DestDir: "{app}\distort_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\dnalogo_scr\dnalogo.scr"; DestDir: "{app}\dnalogo_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\drift_scr\drift.scr"; DestDir: "{app}\drift_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\endgame_scr\endgame.scr"; DestDir: "{app}\endgame_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\epicycle_scr\epicycle.scr"; DestDir: "{app}\epicycle_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\eruption_scr\eruption.scr"; DestDir: "{app}\eruption_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\euler2d_scr\euler2d.scr"; DestDir: "{app}\euler2d_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\extrusion_scr\extrusion.scr"; DestDir: "{app}\extrusion_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\fadeplot_scr\fadeplot.scr"; DestDir: "{app}\fadeplot_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\fiberlamp_scr\fiberlamp.scr"; DestDir: "{app}\fiberlamp_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flag_scr\flag.scr"; DestDir: "{app}\flag_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flame_scr\flame.scr"; DestDir: "{app}\flame_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flipflop_scr\flipflop.scr"; DestDir: "{app}\flipflop_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flipscreen3d_scr\flipscreen3d.scr"; DestDir: "{app}\flipscreen3d_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flow_scr\flow.scr"; DestDir: "{app}\flow_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\fluidballs_scr\fluidballs.scr"; DestDir: "{app}\fluidballs_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flurry_scr\flurry.scr"; DestDir: "{app}\flurry_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\flyingtoasters_scr\flyingtoasters.scr"; DestDir: "{app}\flyingtoasters_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\forest_scr\forest.scr"; DestDir: "{app}\forest_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\fuzzyflakes_scr\fuzzyflakes.scr"; DestDir: "{app}\fuzzyflakes_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\galaxy_scr\galaxy.scr"; DestDir: "{app}\galaxy_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\gears_scr\gears.scr"; DestDir: "{app}\gears_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\gflux_scr\gflux.scr"; DestDir: "{app}\gflux_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glblur_scr\glblur.scr"; DestDir: "{app}\glblur_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glcells_scr\glcells.scr"; DestDir: "{app}\glcells_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\gleidescope_scr\gleidescope.scr"; DestDir: "{app}\gleidescope_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glforestfire_scr\glforestfire.scr"; DestDir: "{app}\glforestfire_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glhanoi_scr\glhanoi.scr"; DestDir: "{app}\glhanoi_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glknots_scr\glknots.scr"; DestDir: "{app}\glknots_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glmatrix_scr\glmatrix.scr"; DestDir: "{app}\glmatrix_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glplanet_scr\glplanet.scr"; DestDir: "{app}\glplanet_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\glschool_scr\glschool.scr"; DestDir: "{app}\glschool_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\goop_scr\goop.scr"; DestDir: "{app}\goop_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\grav_scr\grav.scr"; DestDir: "{app}\grav_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\greynetic_scr\greynetic.scr"; DestDir: "{app}\greynetic_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\halftone_scr\halftone.scr"; DestDir: "{app}\halftone_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\helix_scr\helix.scr"; DestDir: "{app}\helix_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hexadrop_scr\hexadrop.scr"; DestDir: "{app}\hexadrop_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hilbert_scr\hilbert.scr"; DestDir: "{app}\hilbert_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hopalong_scr\hopalong.scr"; DestDir: "{app}\hopalong_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hyperball_scr\hyperball.scr"; DestDir: "{app}\hyperball_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hypercube_scr\hypercube.scr"; DestDir: "{app}\hypercube_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hypertorus_scr\hypertorus.scr"; DestDir: "{app}\hypertorus_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\hypnowheel_scr\hypnowheel.scr"; DestDir: "{app}\hypnowheel_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\ifs_scr\ifs.scr"; DestDir: "{app}\ifs_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\imsmap_scr\imsmap.scr"; DestDir: "{app}\imsmap_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\interference_scr\interference.scr"; DestDir: "{app}\interference_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\intermomentary_scr\intermomentary.scr"; DestDir: "{app}\intermomentary_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\jigglypuff_scr\jigglypuff.scr"; DestDir: "{app}\jigglypuff_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\julia_scr\julia.scr"; DestDir: "{app}\julia_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\kaleidocycle_scr\kaleidocycle.scr"; DestDir: "{app}\kaleidocycle_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\klein_scr\klein.scr"; DestDir: "{app}\klein_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\kumppa_scr\kumppa.scr"; DestDir: "{app}\kumppa_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lament_scr\lament.scr"; DestDir: "{app}\lament_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\laser_scr\laser.scr"; DestDir: "{app}\laser_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lcdscrub_scr\lcdscrub.scr"; DestDir: "{app}\lcdscrub_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lightning_scr\lightning.scr"; DestDir: "{app}\lightning_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lisa_scr\lisa.scr"; DestDir: "{app}\lisa_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lissie_scr\lissie.scr"; DestDir: "{app}\lissie_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lmorph_scr\lmorph.scr"; DestDir: "{app}\lmorph_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\lockward_scr\lockward.scr"; DestDir: "{app}\lockward_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\loop_scr\loop.scr"; DestDir: "{app}\loop_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\maze_scr\maze.scr"; DestDir: "{app}\maze_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\memscroller_scr\memscroller.scr"; DestDir: "{app}\memscroller_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\menger_scr\menger.scr"; DestDir: "{app}\menger_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\metaballs_scr\metaballs.scr"; DestDir: "{app}\metaballs_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\mirrorblob_scr\mirrorblob.scr"; DestDir: "{app}\mirrorblob_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\moebiusgears_scr\moebiusgears.scr"; DestDir: "{app}\moebiusgears_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\moebius_scr\moebius.scr"; DestDir: "{app}\moebius_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\moire_scr\moire.scr"; DestDir: "{app}\moire_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\morph3d_scr\morph3d.scr"; DestDir: "{app}\morph3d_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\mountain_scr\mountain.scr"; DestDir: "{app}\mountain_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\munch_scr\munch.scr"; DestDir: "{app}\munch_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\nerverot_scr\nerverot.scr"; DestDir: "{app}\nerverot_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\noof_scr\noof.scr"; DestDir: "{app}\noof_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\pedal_scr\pedal.scr"; DestDir: "{app}\pedal_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\penetrate_scr\penetrate.scr"; DestDir: "{app}\penetrate_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\penrose_scr\penrose.scr"; DestDir: "{app}\penrose_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\petri_scr\petri.scr"; DestDir: "{app}\petri_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\pipes_scr\pipes.scr"; DestDir: "{app}\pipes_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\polytopes_scr\polytopes.scr"; DestDir: "{app}\polytopes_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\popsquares_scr\popsquares.scr"; DestDir: "{app}\popsquares_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\providence_scr\providence.scr"; DestDir: "{app}\providence_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\pulsar_scr\pulsar.scr"; DestDir: "{app}\pulsar_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\queens_scr\queens.scr"; DestDir: "{app}\queens_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\rd-bomb_scr\rd-bomb.scr"; DestDir: "{app}\rd-bomb_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\rocks_scr\rocks.scr"; DestDir: "{app}\rocks_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\rorschach_scr\rorschach.scr"; DestDir: "{app}\rorschach_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\rotor_scr\rotor.scr"; DestDir: "{app}\rotor_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\rubikblocks_scr\rubikblocks.scr"; DestDir: "{app}\rubikblocks_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\rubik_scr\rubik.scr"; DestDir: "{app}\rubik_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\sballs_scr\sballs.scr"; DestDir: "{app}\sballs_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\shadebobs_scr\shadebobs.scr"; DestDir: "{app}\shadebobs_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\sierpinski3d_scr\sierpinski3d.scr"; DestDir: "{app}\sierpinski3d_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\sierpinski_scr\sierpinski.scr"; DestDir: "{app}\sierpinski_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\skytentacles_scr\skytentacles.scr"; DestDir: "{app}\skytentacles_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\sphere_scr\sphere.scr"; DestDir: "{app}\sphere_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\spiral_scr\spiral.scr"; DestDir: "{app}\spiral_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\sproingies_scr\sproingies.scr"; DestDir: "{app}\sproingies_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\squiral_scr\squiral.scr"; DestDir: "{app}\squiral_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\stairs_scr\stairs.scr"; DestDir: "{app}\stairs_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\starfish_scr\starfish.scr"; DestDir: "{app}\starfish_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\stonerview_scr\stonerview.scr"; DestDir: "{app}\stonerview_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\superquadrics_scr\superquadrics.scr"; DestDir: "{app}\superquadrics_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\surfaces_scr\surfaces.scr"; DestDir: "{app}\surfaces_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\thornbird_scr\thornbird.scr"; DestDir: "{app}\thornbird_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\topblock_scr\topblock.scr"; DestDir: "{app}\topblock_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\triangle_scr\triangle.scr"; DestDir: "{app}\triangle_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\tronbit_scr\tronbit.scr"; DestDir: "{app}\tronbit_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\truchet_scr\truchet.scr"; DestDir: "{app}\truchet_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\twang_scr\twang.scr"; DestDir: "{app}\twang_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\unknownpleasures_scr\unknownpleasures.scr"; DestDir: "{app}\unknownpleasures_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\vines_scr\vines.scr"; DestDir: "{app}\vines_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\voronoi_scr\voronoi.scr"; DestDir: "{app}\voronoi_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\wander_scr\wander.scr"; DestDir: "{app}\wander_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\whirlwindwarp_scr\whirlwindwarp.scr"; DestDir: "{app}\whirlwindwarp_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\whirlygig_scr\whirlygig.scr"; DestDir: "{app}\whirlygig_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\wormhole_scr\wormhole.scr"; DestDir: "{app}\wormhole_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\xflame_scr\xflame.scr"; DestDir: "{app}\xflame_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\xjack_scr\xjack.scr"; DestDir: "{app}\xjack_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\xlyap_scr\xlyap.scr"; DestDir: "{app}\xlyap_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\xrayswarm_scr\xrayswarm.scr"; DestDir: "{app}\xrayswarm_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\xspirograph_scr\xspirograph.scr"; DestDir: "{app}\xspirograph_scr"; Flags: ignoreversion
Source: "XScreenSaverWin\zoom_scr\zoom.scr"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\XScreenSaver for Windows"; Filename: "{app}\xscreensaver.exe"
Name: "{group}\{cm:ProgramOnTheWeb,XScreenSaver for Windows}"; Filename: "http://katahiromz.web.fc2.com/xscreensaverwin/eindex.html"
Name: "{group}\{cm:UninstallProgram,XScreenSaver for Windows}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\XScreenSaver for Windows"; Filename: "{app}\xscreensaver.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\xscreensaver.exe"; Description: "{cm:LaunchProgram,XScreenSaver for Windows}"; Flags: nowait postinstall skipifsilent
