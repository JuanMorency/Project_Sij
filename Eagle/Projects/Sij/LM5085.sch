<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.6.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.025" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="yes" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="yes" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="yes" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="yes" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="yes" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="yes" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="yes" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="yes" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="yes" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="yes" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="yes" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="yes" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="yes" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="yes" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="yes" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="yes" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="yes" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="yes" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="yes" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="yes" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="yes" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="yes" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic>
<libraries>
<library name="LM5085">
<packages>
<package name="0805">
<smd name="1" x="-0.9017" y="0" dx="1.1176" dy="1.4478" layer="1"/>
<smd name="2" x="0.9017" y="0" dx="1.1176" dy="1.4478" layer="1"/>
<wire x1="-0.3556" y1="-0.7112" x2="-0.3556" y2="0.7112" width="0.1524" layer="25"/>
<wire x1="-0.3556" y1="0.7112" x2="-1.1176" y2="0.7112" width="0.1524" layer="25"/>
<wire x1="-1.1176" y1="-0.7112" x2="-0.3556" y2="-0.7112" width="0.1524" layer="25"/>
<wire x1="0.3556" y1="0.7112" x2="0.3556" y2="-0.7112" width="0.1524" layer="25"/>
<wire x1="0.3556" y1="-0.7112" x2="1.1176" y2="-0.7112" width="0.1524" layer="25"/>
<wire x1="1.1176" y1="0.7112" x2="1.1176" y2="0.7366" width="0.1524" layer="25"/>
<wire x1="1.1176" y1="0.7366" x2="0.3556" y2="0.7112" width="0.1524" layer="25"/>
<wire x1="-0.3556" y1="-0.7112" x2="0.3556" y2="-0.7112" width="0.1524" layer="25"/>
<wire x1="1.1176" y1="-0.7112" x2="1.1176" y2="0.7112" width="0.1524" layer="25"/>
<wire x1="1.1176" y1="0.7112" x2="-0.3556" y2="0.7112" width="0.1524" layer="25"/>
<wire x1="-1.1176" y1="0.7112" x2="-1.1176" y2="-0.7112" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="0402">
<smd name="1" x="-0.4191" y="0" dx="0.5588" dy="0.6096" layer="1"/>
<smd name="2" x="0.4191" y="0" dx="0.5588" dy="0.6096" layer="1"/>
<wire x1="-0.1524" y1="-0.3048" x2="-0.1524" y2="0.3048" width="0.1524" layer="25"/>
<wire x1="-0.1524" y1="0.3048" x2="-0.5588" y2="0.3048" width="0.1524" layer="25"/>
<wire x1="-0.5588" y1="-0.3048" x2="-0.1524" y2="-0.3048" width="0.1524" layer="25"/>
<wire x1="0.127" y1="0.3048" x2="0.1524" y2="-0.3048" width="0.1524" layer="25"/>
<wire x1="0.1524" y1="-0.3048" x2="0.5588" y2="-0.3048" width="0.1524" layer="25"/>
<wire x1="0.5588" y1="0.3048" x2="0.127" y2="0.3048" width="0.1524" layer="25"/>
<wire x1="-0.1524" y1="-0.3048" x2="0.1524" y2="-0.3048" width="0.1524" layer="25"/>
<wire x1="0.5588" y1="-0.3048" x2="0.5588" y2="0.3048" width="0.1524" layer="25"/>
<wire x1="0.127" y1="0.3048" x2="-0.1524" y2="0.3048" width="0.1524" layer="25"/>
<wire x1="-0.5588" y1="0.3048" x2="-0.5588" y2="-0.3048" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="MUY08A">
<smd name="1" x="-2.1971" y="0.975" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="2" x="-2.1971" y="0.325" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="3" x="-2.1971" y="-0.325" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="4" x="-2.1971" y="-0.975" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="5" x="2.1971" y="-0.975" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="6" x="2.1971" y="-0.325" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="7" x="2.1971" y="0.325" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="8" x="2.1971" y="0.975" dx="1.3716" dy="0.4572" layer="1"/>
<smd name="9" x="0" y="0" dx="1.8796" dy="2.0066" layer="1"/>
<wire x1="-1.5494" y1="-1.5494" x2="1.5494" y2="-1.5494" width="0.1524" layer="21"/>
<wire x1="1.5494" y1="1.5494" x2="-1.5494" y2="1.5494" width="0.1524" layer="21"/>
<text x="-3.0226" y="0.8128" size="1.27" layer="21" ratio="6" rot="SR0">*</text>
<wire x1="-1.5494" y1="0.762" x2="-1.5494" y2="1.1684" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="1.1684" x2="-2.54" y2="1.1684" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="1.1684" x2="-2.54" y2="0.762" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="0.762" x2="-1.5494" y2="0.762" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="0.127" x2="-1.5494" y2="0.5334" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="0.5334" x2="-2.54" y2="0.5334" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="0.5334" x2="-2.54" y2="0.127" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="0.127" x2="-1.5494" y2="0.127" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="-0.5334" x2="-1.5494" y2="-0.127" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="-0.127" x2="-2.54" y2="-0.127" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="-0.127" x2="-2.54" y2="-0.5334" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="-0.5334" x2="-1.5494" y2="-0.5334" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="-1.1684" x2="-1.5494" y2="-0.762" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="-0.762" x2="-2.54" y2="-0.762" width="0.1524" layer="25"/>
<wire x1="-2.54" y1="-0.762" x2="-2.5146" y2="-1.1684" width="0.1524" layer="25"/>
<wire x1="-2.5146" y1="-1.1684" x2="-1.5494" y2="-1.1684" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="-0.762" x2="1.5494" y2="-1.1684" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="-1.1684" x2="2.54" y2="-1.1684" width="0.1524" layer="25"/>
<wire x1="2.54" y1="-1.1684" x2="2.54" y2="-0.762" width="0.1524" layer="25"/>
<wire x1="2.54" y1="-0.762" x2="1.5494" y2="-0.762" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="-0.127" x2="1.5494" y2="-0.5334" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="-0.5334" x2="2.54" y2="-0.5334" width="0.1524" layer="25"/>
<wire x1="2.54" y1="-0.5334" x2="2.54" y2="-0.127" width="0.1524" layer="25"/>
<wire x1="2.54" y1="-0.127" x2="1.5494" y2="-0.127" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="0.5334" x2="1.5494" y2="0.127" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="0.127" x2="2.54" y2="0.127" width="0.1524" layer="25"/>
<wire x1="2.54" y1="0.127" x2="2.54" y2="0.5334" width="0.1524" layer="25"/>
<wire x1="2.54" y1="0.5334" x2="1.5494" y2="0.5334" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="1.1684" x2="1.5494" y2="0.762" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="0.762" x2="2.54" y2="0.762" width="0.1524" layer="25"/>
<wire x1="2.54" y1="0.762" x2="2.54" y2="1.1684" width="0.1524" layer="25"/>
<wire x1="2.54" y1="1.1684" x2="1.5494" y2="1.1684" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="1.5494" x2="-1.5494" y2="-1.5494" width="0.1524" layer="25"/>
<wire x1="-1.5494" y1="-1.5494" x2="1.5494" y2="-1.5494" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="-1.5494" x2="1.5494" y2="1.5494" width="0.1524" layer="25"/>
<wire x1="1.5494" y1="1.5494" x2="0.3048" y2="1.5494" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="1.5494" x2="-0.3048" y2="1.5494" width="0.1524" layer="25"/>
<wire x1="-0.3048" y1="1.5494" x2="-1.5494" y2="1.5494" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="1.5494" x2="-0.3048" y2="1.5494" width="0.1524" layer="25" curve="-180"/>
<text x="-3.0226" y="0.8128" size="1.27" layer="25" ratio="6" rot="SR0">*</text>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="0603">
<smd name="1" x="-0.6985" y="0" dx="0.6604" dy="0.9652" layer="1"/>
<smd name="2" x="0.6985" y="0" dx="0.6604" dy="0.9652" layer="1"/>
<wire x1="-0.3556" y1="-0.4826" x2="-0.3556" y2="0.4826" width="0.1524" layer="25"/>
<wire x1="-0.3556" y1="0.4826" x2="-0.8636" y2="0.4826" width="0.1524" layer="25"/>
<wire x1="-0.8636" y1="-0.4826" x2="-0.3556" y2="-0.4826" width="0.1524" layer="25"/>
<wire x1="0.3556" y1="0.4826" x2="0.3556" y2="-0.4826" width="0.1524" layer="25"/>
<wire x1="0.3556" y1="-0.4826" x2="0.8636" y2="-0.4826" width="0.1524" layer="25"/>
<wire x1="0.8636" y1="0.4826" x2="0.3556" y2="0.4826" width="0.1524" layer="25"/>
<wire x1="-0.3556" y1="-0.4826" x2="0.3556" y2="-0.4826" width="0.1524" layer="25"/>
<wire x1="0.8636" y1="-0.4826" x2="0.8636" y2="0.4826" width="0.1524" layer="25"/>
<wire x1="0.3556" y1="0.4826" x2="-0.3556" y2="0.4826" width="0.1524" layer="25"/>
<wire x1="-0.8636" y1="0.4826" x2="-0.8636" y2="-0.4826" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="0201">
<smd name="1" x="-0.2921" y="0" dx="0.3556" dy="0.3302" layer="1"/>
<smd name="2" x="0.2921" y="0" dx="0.3556" dy="0.3302" layer="1"/>
<wire x1="-0.1016" y1="-0.1524" x2="-0.1016" y2="0.1524" width="0.1524" layer="25"/>
<wire x1="-0.1016" y1="0.1524" x2="-0.3048" y2="0.1524" width="0.1524" layer="25"/>
<wire x1="-0.3048" y1="-0.1524" x2="-0.1016" y2="-0.1524" width="0.1524" layer="25"/>
<wire x1="0.1016" y1="0.1778" x2="0.127" y2="-0.1524" width="0.1524" layer="25"/>
<wire x1="0.127" y1="-0.1524" x2="0.3048" y2="-0.1524" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="0.1524" x2="0.3048" y2="0.1778" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="0.1778" x2="0.1016" y2="0.1778" width="0.1524" layer="25"/>
<wire x1="-0.1016" y1="-0.1524" x2="0.127" y2="-0.1524" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="-0.1524" x2="0.3048" y2="0.1524" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="0.1524" x2="-0.1016" y2="0.1524" width="0.1524" layer="25"/>
<wire x1="-0.3048" y1="0.1524" x2="-0.3048" y2="-0.1524" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="1206">
<smd name="1" x="-1.4986" y="0" dx="1.1176" dy="1.8034" layer="1"/>
<smd name="2" x="1.4986" y="0" dx="1.1176" dy="1.8034" layer="1"/>
<wire x1="-0.6096" y1="-0.9144" x2="0.6096" y2="-0.9144" width="0.1524" layer="21"/>
<wire x1="0.6096" y1="0.9144" x2="-0.6096" y2="0.9144" width="0.1524" layer="21"/>
<wire x1="-0.9398" y1="-0.9144" x2="-0.9398" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="-0.9398" y1="0.9144" x2="-1.7018" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="-1.7018" y1="-0.9144" x2="-0.9398" y2="-0.9144" width="0.1524" layer="25"/>
<wire x1="0.9398" y1="0.9144" x2="0.9398" y2="-0.9144" width="0.1524" layer="25"/>
<wire x1="0.9398" y1="-0.9144" x2="1.7018" y2="-0.889" width="0.1524" layer="25"/>
<wire x1="1.7018" y1="-0.889" x2="1.7018" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="1.7018" y1="0.9144" x2="0.9398" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="-0.9398" y1="-0.9144" x2="1.7018" y2="-0.9144" width="0.1524" layer="25"/>
<wire x1="1.7018" y1="-0.9144" x2="1.7018" y2="-0.889" width="0.1524" layer="25"/>
<wire x1="0.9398" y1="0.9144" x2="-0.9398" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="-1.7018" y1="0.9144" x2="-1.7018" y2="-0.9144" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="POWERPAK_SO-8">
<smd name="1" x="-2.921" y="1.905" dx="1.016" dy="0.508" layer="1"/>
<smd name="2" x="-2.921" y="0.635" dx="1.016" dy="0.508" layer="1"/>
<smd name="3" x="-2.921" y="-0.635" dx="1.016" dy="0.508" layer="1"/>
<smd name="4" x="-2.921" y="-1.905" dx="1.016" dy="0.508" layer="1"/>
<smd name="5" x="2.921" y="-1.905" dx="1.016" dy="0.508" layer="1"/>
<smd name="6" x="2.921" y="-0.635" dx="1.016" dy="0.508" layer="1"/>
<smd name="7" x="2.921" y="0.635" dx="1.016" dy="0.508" layer="1"/>
<smd name="8" x="2.921" y="1.905" dx="1.016" dy="0.508" layer="1"/>
<smd name="9" x="1.2065" y="0" dx="3.2258" dy="3.9878" layer="1"/>
<wire x1="-2.9972" y1="-2.4892" x2="2.9972" y2="-2.4892" width="0.1524" layer="21"/>
<wire x1="2.9972" y1="2.4892" x2="-0.3048" y2="2.4892" width="0.1524" layer="21"/>
<wire x1="-0.3048" y1="2.4892" x2="-2.9972" y2="2.4892" width="0.1524" layer="21"/>
<wire x1="0.3048" y1="2.5146" x2="-0.3048" y2="2.4892" width="0.1524" layer="21" curve="-180"/>
<wire x1="-2.9972" y1="-2.4892" x2="2.9972" y2="-2.4892" width="0.1524" layer="25"/>
<wire x1="2.9972" y1="-2.4892" x2="2.9972" y2="2.4892" width="0.1524" layer="25"/>
<wire x1="2.9972" y1="2.4892" x2="-0.3048" y2="2.4892" width="0.1524" layer="25"/>
<wire x1="-0.3048" y1="2.4892" x2="-2.9972" y2="2.4892" width="0.1524" layer="25"/>
<wire x1="-2.9972" y1="2.4892" x2="-2.9972" y2="-2.4892" width="0.1524" layer="25"/>
<wire x1="0.3048" y1="2.5146" x2="-0.3048" y2="2.4892" width="0.1524" layer="25" curve="-180"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="DDPAK">
<smd name="1" x="-2.54" y="-8.835" dx="1.8308" dy="3.5012" layer="1"/>
<smd name="2" x="0" y="0.1524" dx="10.67" dy="11.6048" layer="1"/>
<smd name="3" x="2.54" y="-8.835" dx="1.8308" dy="3.5012" layer="1"/>
<wire x1="-1.651" y1="-5.6388" x2="-3.429" y2="-5.6388" width="0.1524" layer="25"/>
<wire x1="-3.429" y1="-5.6388" x2="-3.429" y2="-10.2362" width="0.1524" layer="25"/>
<wire x1="-3.429" y1="-10.2362" x2="-1.651" y2="-10.2362" width="0.1524" layer="25"/>
<wire x1="-1.651" y1="-10.2362" x2="-1.651" y2="-5.6388" width="0.1524" layer="25"/>
<wire x1="3.429" y1="-5.6388" x2="1.651" y2="-5.6388" width="0.1524" layer="25"/>
<wire x1="1.651" y1="-5.6388" x2="1.651" y2="-10.2362" width="0.1524" layer="25"/>
<wire x1="1.651" y1="-10.2362" x2="3.429" y2="-10.2362" width="0.1524" layer="25"/>
<wire x1="3.429" y1="-10.2362" x2="3.429" y2="-5.6388" width="0.1524" layer="25"/>
<wire x1="-0.889" y1="-5.6388" x2="-0.889" y2="-7.4422" width="0.1524" layer="25"/>
<wire x1="-0.889" y1="-7.4422" x2="0.889" y2="-7.4422" width="0.1524" layer="25"/>
<wire x1="0.889" y1="-7.4422" x2="0.889" y2="-5.6388" width="0.1524" layer="25"/>
<wire x1="-5.334" y1="-5.6388" x2="5.334" y2="-5.6388" width="0.1524" layer="25"/>
<wire x1="5.334" y1="-5.6388" x2="5.334" y2="5.6388" width="0.1524" layer="25"/>
<wire x1="5.334" y1="5.6388" x2="-5.334" y2="5.6388" width="0.1524" layer="25"/>
<wire x1="-5.334" y1="5.6388" x2="-5.334" y2="-5.6388" width="0.1524" layer="25"/>
<text x="-6.2992" y="-9.4742" size="1.27" layer="25" ratio="6" rot="SR0">*</text>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<wire x1="-3.429" y1="-6.2484" x2="-3.429" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="1.651" y1="-6.2484" x2="1.651" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="-1.651" y1="-6.2484" x2="-1.651" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="3.429" y1="-6.2484" x2="3.429" y2="-6.731" width="0.1524" layer="21"/>
<text x="-6.2992" y="-9.4742" size="1.27" layer="21" ratio="6" rot="SR0">*</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="XAL6030">
<smd name="1" x="-2.0193" y="0" dx="1.4224" dy="5.6896" layer="1"/>
<smd name="2" x="2.0193" y="0" dx="1.4224" dy="5.6896" layer="1"/>
<wire x1="-3.2766" y1="-3.3782" x2="3.2766" y2="-3.3782" width="0.1524" layer="21"/>
<wire x1="3.2766" y1="-3.3782" x2="3.2766" y2="3.3782" width="0.1524" layer="21"/>
<wire x1="3.2766" y1="3.3782" x2="-3.2766" y2="3.3782" width="0.1524" layer="21"/>
<wire x1="-3.2766" y1="3.3782" x2="-3.2766" y2="-3.3782" width="0.1524" layer="21"/>
<wire x1="-3.2766" y1="-3.3782" x2="3.2766" y2="-3.3782" width="0.1524" layer="25"/>
<wire x1="3.2766" y1="-3.3782" x2="3.2766" y2="3.3782" width="0.1524" layer="25"/>
<wire x1="3.2766" y1="3.3782" x2="-3.2766" y2="3.3782" width="0.1524" layer="25"/>
<wire x1="-3.2766" y1="3.3782" x2="-3.2766" y2="-3.3782" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
<package name="CAPSMT_6_A09">
<smd name="1" x="-1.2954" y="0" dx="1.6002" dy="1.397" layer="1"/>
<smd name="2" x="1.2954" y="0" dx="1.6002" dy="1.397" layer="1"/>
<wire x1="-2.4892" y1="0" x2="-3.2512" y2="0" width="0.1524" layer="21"/>
<wire x1="-2.8448" y1="0.381" x2="-2.8448" y2="-0.381" width="0.1524" layer="21"/>
<wire x1="-0.2286" y1="-0.9144" x2="0.2286" y2="-0.9144" width="0.1524" layer="21"/>
<wire x1="0.2286" y1="0.9144" x2="-0.2286" y2="0.9144" width="0.1524" layer="21"/>
<wire x1="-2.4892" y1="0" x2="-3.2512" y2="0" width="0.1524" layer="25"/>
<wire x1="-2.8448" y1="0.381" x2="-2.8448" y2="-0.381" width="0.1524" layer="25"/>
<wire x1="-1.7018" y1="-0.9144" x2="1.7018" y2="-0.9144" width="0.1524" layer="25"/>
<wire x1="1.7018" y1="-0.9144" x2="1.7018" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="1.7018" y1="0.9144" x2="-1.7018" y2="0.9144" width="0.1524" layer="25"/>
<wire x1="-1.7018" y1="0.9144" x2="-1.7018" y2="-0.9144" width="0.1524" layer="25"/>
<text x="-2.8702" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Value</text>
</package>
</packages>
<symbols>
<symbol name="WB_CAPACITOR">
<pin name="1" x="7.62" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="2" x="-7.62" y="0" visible="off" length="short" direction="pas"/>
<wire x1="0.762" y1="-3.048" x2="0.762" y2="0" width="0.1524" layer="94"/>
<wire x1="0.762" y1="0" x2="0.762" y2="3.048" width="0.1524" layer="94"/>
<wire x1="-7.62" y1="0" x2="-0.762" y2="0" width="0.1524" layer="94"/>
<wire x1="0.762" y1="0" x2="7.62" y2="0" width="0.1524" layer="94"/>
<wire x1="-0.762" y1="-3.048" x2="-0.762" y2="3.048" width="0.1524" layer="94"/>
<text x="-4.7244" y="6.5786" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-1.0414" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="WB_RESISTOR">
<pin name="1" x="7.62" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="2" x="-7.62" y="0" visible="off" length="short" direction="pas"/>
<wire x1="7.62" y1="0" x2="5.08" y2="0" width="0.1524" layer="94"/>
<wire x1="5.08" y1="0" x2="3.81" y2="1.524" width="0.1524" layer="94"/>
<wire x1="3.81" y1="1.524" x2="1.905" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="1.905" y1="-1.524" x2="0" y2="1.524" width="0.1524" layer="94"/>
<wire x1="0" y1="1.524" x2="-1.905" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="-1.905" y1="-1.524" x2="-3.81" y2="1.524" width="0.1524" layer="94"/>
<wire x1="-3.81" y1="1.524" x2="-5.08" y2="0" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="0" x2="-7.62" y2="0" width="0.1524" layer="94"/>
<text x="-4.7244" y="4.6736" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-1.0414" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="LM5085MY/NOPB">
<pin name="ADJ" x="25.4" y="15.24" length="short" direction="pas" rot="R180"/>
<pin name="RT" x="-25.4" y="-2.54" length="short" direction="pas"/>
<pin name="FB" x="25.4" y="-30.48" length="short" direction="pas" rot="R180"/>
<pin name="GND_2" x="-25.4" y="-22.86" length="short" direction="pas"/>
<pin name="ISEN" x="25.4" y="0" length="short" direction="pas" rot="R180"/>
<pin name="PGATE" x="25.4" y="-15.24" length="short" direction="pas" rot="R180"/>
<pin name="VCC" x="25.4" y="30.48" length="short" direction="pas" rot="R180"/>
<pin name="VIN" x="-25.4" y="22.86" length="short" direction="pas"/>
<wire x1="-22.86" y1="33.528" x2="22.86" y2="33.528" width="0.1524" layer="94"/>
<wire x1="22.86" y1="33.528" x2="22.86" y2="30.48" width="0.1524" layer="94"/>
<wire x1="22.86" y1="30.48" x2="22.86" y2="15.24" width="0.1524" layer="94"/>
<wire x1="22.86" y1="15.24" x2="22.86" y2="0" width="0.1524" layer="94"/>
<wire x1="22.86" y1="0" x2="22.86" y2="-15.24" width="0.1524" layer="94"/>
<wire x1="22.86" y1="-15.24" x2="22.86" y2="-30.48" width="0.1524" layer="94"/>
<wire x1="22.86" y1="-30.48" x2="22.86" y2="-33.528" width="0.1524" layer="94"/>
<wire x1="22.86" y1="-33.528" x2="-22.86" y2="-33.528" width="0.1524" layer="94"/>
<wire x1="-22.86" y1="-33.528" x2="-22.86" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="-22.86" y1="-22.86" x2="-22.86" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-22.86" y1="-2.54" x2="-22.86" y2="22.86" width="0.1524" layer="94"/>
<wire x1="-22.86" y1="22.86" x2="-22.86" y2="33.528" width="0.1524" layer="94"/>
<wire x1="25.4" y1="15.24" x2="22.86" y2="15.24" width="0.1524" layer="94"/>
<wire x1="-25.4" y1="-2.54" x2="-22.86" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="25.4" y1="-30.48" x2="22.86" y2="-30.48" width="0.1524" layer="94"/>
<wire x1="-25.4" y1="-22.86" x2="-22.86" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="25.4" y1="0" x2="22.86" y2="0" width="0.1524" layer="94"/>
<wire x1="25.4" y1="-15.24" x2="22.86" y2="-15.24" width="0.1524" layer="94"/>
<wire x1="25.4" y1="30.48" x2="22.86" y2="30.48" width="0.1524" layer="94"/>
<wire x1="-25.4" y1="22.86" x2="-22.86" y2="22.86" width="0.1524" layer="94"/>
<text x="-4.7244" y="2.7686" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-4.8514" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="SI7149DP">
<pin name="DRAIN_2" x="3.81" y="-7.62" visible="off" length="short" direction="pas" rot="R90"/>
<pin name="GATE" x="-3.81" y="0" visible="off" length="short" direction="pas"/>
<pin name="SOURCE_2" x="3.81" y="7.62" visible="off" length="short" direction="pas" rot="R270"/>
<wire x1="-3.81" y1="0" x2="0" y2="0" width="0.1524" layer="94"/>
<wire x1="1.016" y1="2.032" x2="1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="1.016" y1="0" x2="1.016" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="1.016" y1="-2.032" x2="1.016" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="3.81" y1="2.032" x2="1.016" y2="2.032" width="0.1524" layer="94"/>
<wire x1="1.27" y1="-2.032" x2="3.556" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="1.016" y1="2.54" x2="1.016" y2="2.032" width="0.1524" layer="94"/>
<wire x1="1.016" y1="-2.032" x2="3.81" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="2.54" y1="0" x2="1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="3.81" y1="2.032" x2="3.81" y2="0" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="2.54" width="0.1524" layer="94"/>
<wire x1="3.81" y1="7.62" x2="3.81" y2="2.032" width="0.1524" layer="94"/>
<wire x1="3.81" y1="-7.62" x2="3.81" y2="-2.032" width="0.1524" layer="94"/>
<polygon width="0" layer="94">
<vertex x="2.54" y="1.27"/>
<vertex x="3.81" y="0"/>
<vertex x="2.54" y="-1.27"/>
</polygon>
<text x="-8.5344" y="5.9436" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-1.0414" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="MBRB745PBF">
<pin name="A_2" x="7.62" y="0" length="short" direction="pas" rot="R180"/>
<pin name="K" x="-7.62" y="0" length="short" direction="pas"/>
<wire x1="-7.62" y1="0" x2="7.62" y2="0" width="0.1524" layer="94"/>
<wire x1="-2.032" y1="2.54" x2="-2.032" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-2.032" y1="2.54" x2="-2.54" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-2.54" y2="2.032" width="0.1524" layer="94"/>
<wire x1="-2.032" y1="-2.54" x2="-1.524" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-1.524" y1="-2.54" x2="-1.524" y2="-2.032" width="0.1524" layer="94"/>
<polygon width="0.1524" layer="94">
<vertex x="2.032" y="2.54"/>
<vertex x="-2.032" y="0"/>
<vertex x="2.032" y="-2.54"/>
</polygon>
<text x="-4.699" y="2.9972" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-1.0414" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="WB_INDUCTOR">
<pin name="2" x="7.62" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="1" x="-7.62" y="0" visible="off" length="short" direction="pas"/>
<wire x1="5.08" y1="0" x2="7.62" y2="0" width="0.1524" layer="94"/>
<wire x1="-7.62" y1="0" x2="-5.08" y2="0" width="0.1524" layer="94"/>
<wire x1="2.54" y1="0" x2="2.6924" y2="0.8382" width="0.1524" layer="94" curve="-12"/>
<wire x1="2.6924" y1="0.8382" x2="3.81" y2="1.8542" width="0.1524" layer="94" curve="-78"/>
<wire x1="3.81" y1="1.8542" x2="4.9022" y2="0.8382" width="0.1524" layer="94" curve="-79"/>
<wire x1="4.9022" y1="0.8382" x2="5.08" y2="0.0762" width="0.1524" layer="94" curve="-2"/>
<wire x1="0" y1="0" x2="0.1524" y2="0.8382" width="0.1524" layer="94" curve="-12"/>
<wire x1="0.1524" y1="0.8382" x2="1.27" y2="1.8542" width="0.1524" layer="94" curve="-78"/>
<wire x1="1.27" y1="1.8542" x2="2.3622" y2="0.8382" width="0.1524" layer="94" curve="-79"/>
<wire x1="2.3622" y1="0.8382" x2="2.54" y2="-0.1778" width="0.1524" layer="94" curve="-17"/>
<wire x1="-2.54" y1="0" x2="-2.3876" y2="0.8382" width="0.1524" layer="94" curve="-12"/>
<wire x1="-2.3876" y1="0.8382" x2="-1.27" y2="1.8542" width="0.1524" layer="94" curve="-78"/>
<wire x1="-1.27" y1="1.8542" x2="-0.1778" y2="0.8382" width="0.1524" layer="94" curve="-79"/>
<wire x1="-0.1778" y1="0.8382" x2="0" y2="-0.1778" width="0.1524" layer="94" curve="-17"/>
<wire x1="-5.08" y1="0" x2="-4.9276" y2="0.8382" width="0.1524" layer="94" curve="-12"/>
<wire x1="-4.9276" y1="0.8382" x2="-3.81" y2="1.8542" width="0.1524" layer="94" curve="-78"/>
<wire x1="-3.81" y1="1.8542" x2="-2.7178" y2="0.8382" width="0.1524" layer="94" curve="-79"/>
<wire x1="-2.7178" y1="0.8382" x2="-2.54" y2="-0.1778" width="0.1524" layer="94" curve="-17"/>
<wire x1="-2.54" y1="-0.1778" x2="-2.54" y2="0" width="0.1524" layer="94" curve="-53"/>
<wire x1="0" y1="-0.1778" x2="0" y2="0" width="0.1524" layer="94" curve="-53"/>
<wire x1="2.54" y1="-0.1778" x2="2.54" y2="0" width="0.1524" layer="94" curve="-53"/>
<text x="-4.7244" y="4.6736" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-1.0414" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="WB_CAP_POLARIZED">
<pin name="+" x="7.62" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="-" x="-7.62" y="0" visible="off" length="short" direction="pas"/>
<wire x1="1.016" y1="-3.302" x2="1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="1.016" y1="0" x2="1.016" y2="3.302" width="0.1524" layer="94"/>
<wire x1="-7.62" y1="0" x2="-0.508" y2="0" width="0.1524" layer="94"/>
<wire x1="1.016" y1="0" x2="7.62" y2="0" width="0.1524" layer="94"/>
<wire x1="3.556" y1="-2.54" x2="3.556" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="3.048" y1="-2.032" x2="4.064" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="-2.286" x2="-1.651" y2="-3.048" width="0.1524" layer="94" curve="-19"/>
<wire x1="-0.381" y1="0" x2="-1.016" y2="-2.286" width="0.1524" layer="94" curve="-33"/>
<wire x1="-1.016" y1="2.286" x2="-0.381" y2="0" width="0.1524" layer="94" curve="-33"/>
<wire x1="-1.651" y1="3.048" x2="-1.016" y2="2.286" width="0.1524" layer="94" curve="-19"/>
<text x="-4.4958" y="7.0104" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="-5.3594" y="-1.0414" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="GRM21BR61E475MA12L">
<gates>
<gate name="A" symbol="WB_CAPACITOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0805">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CAP" value="4.7E-6" constant="no"/>
<attribute name="DATASHEETURL" value="http://www.murata.com/products/catalog/pdf/c02e.pdf" constant="no"/>
<attribute name="ESR" value="0.0020" constant="no"/>
<attribute name="IRMS" value="7.29" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="MuRata" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="GRM21BR61E475MA12L" constant="no"/>
<attribute name="VDC" value="25.0" constant="no"/>
<attribute name="VENDOR" value="MuRata" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CRCW040226K7FKED">
<gates>
<gate name="A" symbol="WB_RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0402">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MANUFACTURER_NAME" value="Vishay-Dale" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="CRCW040226K7FKED" constant="no"/>
<attribute name="POWER" value="0.063" constant="no"/>
<attribute name="RESISTANCE" value="26700.0" constant="no"/>
<attribute name="TOLERANCE" value="1.0" constant="no"/>
<attribute name="VENDOR" value="Vishay-Dale" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="LM5085MY/NOPB">
<gates>
<gate name="A" symbol="LM5085MY/NOPB" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MUY08A">
<connects>
<connect gate="A" pin="ADJ" pad="1"/>
<connect gate="A" pin="FB" pad="3"/>
<connect gate="A" pin="GND_2" pad="4"/>
<connect gate="A" pin="ISEN" pad="5"/>
<connect gate="A" pin="PGATE" pad="6"/>
<connect gate="A" pin="RT" pad="2"/>
<connect gate="A" pin="VCC" pad="7"/>
<connect gate="A" pin="VIN" pad="8"/>
</connects>
<technologies>
<technology name="">
<attribute name="DATASHEETURL" value="http://www.ti.com/product/lm5085" constant="no"/>
<attribute name="FREQUENCY" value="2000000.0" constant="no"/>
<attribute name="IMAX" value="12.0" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Texas Instruments" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="LM5085MY/NOPB" constant="no"/>
<attribute name="MKTG_PACKAGE" value="NA" constant="no"/>
<attribute name="NSID" value="LM5085MM/NOPB" constant="no"/>
<attribute name="PACKAGE" value="NA" constant="no"/>
<attribute name="VENDOR" value="Texas Instruments" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GRM188R61C105KA93D">
<gates>
<gate name="A" symbol="WB_CAPACITOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0603">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CAP" value="1.0E-6" constant="no"/>
<attribute name="ESR" value="0.0" constant="no"/>
<attribute name="IRMS" value="0.0" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="MuRata" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="GRM188R61C105KA93D" constant="no"/>
<attribute name="VDC" value="16.0" constant="no"/>
<attribute name="VENDOR" value="MuRata" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GRM033R71C102KA01D">
<gates>
<gate name="A" symbol="WB_CAPACITOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0201">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CAP" value="1.0E-9" constant="no"/>
<attribute name="ESR" value="0.0" constant="no"/>
<attribute name="IRMS" value="0.0" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="MuRata" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="GRM033R71C102KA01D" constant="no"/>
<attribute name="VDC" value="16.0" constant="no"/>
<attribute name="VENDOR" value="MuRata" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CRCW04022K55FKED">
<gates>
<gate name="A" symbol="WB_RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0402">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MANUFACTURER_NAME" value="Vishay-Dale" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="CRCW04022K55FKED" constant="no"/>
<attribute name="POWER" value="0.063" constant="no"/>
<attribute name="RESISTANCE" value="2550.0" constant="no"/>
<attribute name="TOLERANCE" value="1.0" constant="no"/>
<attribute name="VENDOR" value="Vishay-Dale" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CSR1206FK10L0">
<gates>
<gate name="A" symbol="WB_RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1206">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MANUFACTURER_NAME" value="Stackpole Electronics Inc" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="CSR1206FK10L0" constant="no"/>
<attribute name="POWER" value="0.5" constant="no"/>
<attribute name="RESISTANCE" value="0.01" constant="no"/>
<attribute name="TOLERANCE" value="1.0" constant="no"/>
<attribute name="VENDOR" value="Stackpole Electronics Inc" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SI7149DP">
<gates>
<gate name="A" symbol="SI7149DP" x="0" y="0"/>
</gates>
<devices>
<device name="" package="POWERPAK_SO-8">
<connects>
<connect gate="A" pin="DRAIN_2" pad="5"/>
<connect gate="A" pin="GATE" pad="4"/>
<connect gate="A" pin="SOURCE_2" pad="1"/>
</connects>
<technologies>
<technology name="">
<attribute name="DATASHEETURL" value="http://www.vishay.com/docs/68934/si7149dp.pdf" constant="no"/>
<attribute name="IDSMAX" value="-23.7" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Vishay-Siliconix" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="Si7149DP" constant="no"/>
<attribute name="QGTYP" value="77.0" constant="no"/>
<attribute name="QUANTITY" value="1" constant="no"/>
<attribute name="RDSON45" value="0.0094" constant="no"/>
<attribute name="VDSMAX" value="-30.0" constant="no"/>
<attribute name="VENDOR" value="Vishay-Siliconix" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="MBRB745PBF">
<gates>
<gate name="A" symbol="MBRB745PBF" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DDPAK">
<connects>
<connect gate="A" pin="A_2" pad="1"/>
<connect gate="A" pin="K" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="DATASHEETURL" value="http://www.vishay.com/docs/88680/88680.pdf" constant="no"/>
<attribute name="IO" value="7.5" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Vishay-Semiconductor" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="MBRB745PBF" constant="no"/>
<attribute name="VENDOR" value="Vishay-Semiconductor" constant="no"/>
<attribute name="VFATIO" value="0.7" constant="no"/>
<attribute name="VRRM" value="45.0" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="XAL6030-222MEB">
<gates>
<gate name="A" symbol="WB_INDUCTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="XAL6030">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="DCR" value="0.01" constant="no"/>
<attribute name="IDC" value="12.7" constant="no"/>
<attribute name="L" value="2.2E-6" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Coilcraft" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="XAL6030-222MEB" constant="no"/>
<attribute name="VENDOR" value="Coilcraft" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GRM188R72E681KW07D">
<gates>
<gate name="A" symbol="WB_CAPACITOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0603">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CAP" value="6.8E-10" constant="no"/>
<attribute name="ESR" value="0.0" constant="no"/>
<attribute name="IRMS" value="0.0" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="MuRata" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="GRM188R72E681KW07D" constant="no"/>
<attribute name="VDC" value="250.0" constant="no"/>
<attribute name="VENDOR" value="MuRata" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CRCW04023K32FKED">
<gates>
<gate name="A" symbol="WB_RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0402">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MANUFACTURER_NAME" value="Vishay-Dale" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="CRCW04023K32FKED" constant="no"/>
<attribute name="POWER" value="0.063" constant="no"/>
<attribute name="RESISTANCE" value="3320.0" constant="no"/>
<attribute name="TOLERANCE" value="1.0" constant="no"/>
<attribute name="VENDOR" value="Vishay-Dale" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CRCW040210K0FKED">
<gates>
<gate name="A" symbol="WB_RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="0402">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MANUFACTURER_NAME" value="Vishay-Dale" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="CRCW040210K0FKED" constant="no"/>
<attribute name="POWER" value="0.063" constant="no"/>
<attribute name="RESISTANCE" value="10000.0" constant="no"/>
<attribute name="TOLERANCE" value="1.0" constant="no"/>
<attribute name="VENDOR" value="Vishay-Dale" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="6TPH100MAEA">
<gates>
<gate name="A" symbol="WB_CAP_POLARIZED" x="0" y="0"/>
</gates>
<devices>
<device name="" package="CAPSMT_6_A09">
<connects>
<connect gate="A" pin="+" pad="1"/>
<connect gate="A" pin="-" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CAP" value="1.0E-4" constant="no"/>
<attribute name="DATASHEETURL" value="http://industrial.panasonic.com/www%2Ddata/pdf/AAA8000/AAA8000CE2.pdf" constant="no"/>
<attribute name="ESR" value="0.1" constant="no"/>
<attribute name="IRMS" value="0.67" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Panasonic" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="6TPH100MAEA" constant="no"/>
<attribute name="VDC" value="6.3" constant="no"/>
<attribute name="VENDOR" value="Panasonic" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="CFF" library="LM5085" deviceset="GRM188R72E681KW07D" device="" value="680pF"/>
<part name="CADJ" library="LM5085" deviceset="GRM033R71C102KA01D" device="" value="1nF"/>
<part name="CIN" library="LM5085" deviceset="GRM21BR61E475MA12L" device="" value="4.7uF"/>
<part name="COUT" library="LM5085" deviceset="6TPH100MAEA" device="" value="100uF"/>
<part name="CVCC" library="LM5085" deviceset="GRM188R61C105KA93D" device="" value="1uF"/>
<part name="D1" library="LM5085" deviceset="MBRB745PBF" device="" value="700mV"/>
<part name="L1" library="LM5085" deviceset="XAL6030-222MEB" device="" value="2.2uH"/>
<part name="M1" library="LM5085" deviceset="SI7149DP" device="" value="-30V"/>
<part name="RADJ" library="LM5085" deviceset="CRCW04022K55FKED" device="" value="2.55k"/>
<part name="RFB1" library="LM5085" deviceset="CRCW04023K32FKED" device="" value="3.32k"/>
<part name="RFB2" library="LM5085" deviceset="CRCW040210K0FKED" device="" value="10k"/>
<part name="RSNS" library="LM5085" deviceset="CSR1206FK10L0" device="" value="10m"/>
<part name="RT" library="LM5085" deviceset="CRCW040226K7FKED" device="" value="26.7k"/>
<part name="U1" library="LM5085" deviceset="LM5085MY/NOPB" device="" value="LM5085MY/NOPB"/>
<part name="CIN_2" library="LM5085" deviceset="GRM21BR61E475MA12L" device="" value="4.7uF"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="CFF" gate="A" x="230.505" y="77.47" smashed="yes" rot="R90">
<attribute name="NAME" x="233.553" y="81.788" size="2" layer="95"/>
<attribute name="VALUE" x="233.553" y="77.216" size="2" layer="96"/>
</instance>
<instance part="CADJ" gate="A" x="174.625" y="125.73" smashed="yes">
<attribute name="NAME" x="174.625" y="131.826" size="2" layer="95" align="bottom-center"/>
<attribute name="VALUE" x="174.625" y="118.872" size="2" layer="96" align="bottom-center"/>
</instance>
<instance part="CIN" gate="A" x="76.2" y="91.44" smashed="yes" rot="R90">
<attribute name="NAME" x="79.248" y="95.758" size="2" layer="95"/>
<attribute name="VALUE" x="79.248" y="91.186" size="2" layer="96"/>
</instance>
<instance part="COUT" gate="A" x="256.54" y="58.42" smashed="yes" rot="R90">
<attribute name="NAME" x="259.842" y="62.992" size="2" layer="95"/>
<attribute name="VALUE" x="259.842" y="58.42" size="2" layer="96"/>
</instance>
<instance part="CVCC" gate="A" x="163.83" y="139.7" smashed="yes" rot="R90">
<attribute name="NAME" x="166.878" y="144.018" size="2" layer="95"/>
<attribute name="VALUE" x="166.878" y="139.446" size="2" layer="96"/>
</instance>
<instance part="D1" gate="A" x="209.55" y="77.47" rot="R270"/>
<instance part="L1" gate="A" x="219.71" y="86.36" smashed="yes">
<attribute name="NAME" x="219.71" y="90.678" size="2" layer="95" align="bottom-center"/>
<attribute name="VALUE" x="219.71" y="81.28" size="2" layer="96" align="bottom-center"/>
</instance>
<instance part="M1" gate="A" x="200.66" y="96.52" smashed="yes">
<attribute name="NAME" x="200.66" y="104.14" size="2" layer="95" align="top-center"/>
<attribute name="VALUE" x="200.66" y="88.9" size="2" layer="96" align="bottom-center"/>
</instance>
<instance part="RADJ" gate="A" x="176.53" y="111.76" smashed="yes">
<attribute name="NAME" x="176.53" y="116.332" size="2" layer="95" align="bottom-center"/>
<attribute name="VALUE" x="176.53" y="106.426" size="2" layer="96" align="bottom-center"/>
</instance>
<instance part="RFB1" gate="A" x="244.475" y="56.515" smashed="yes" rot="R90">
<attribute name="NAME" x="245.999" y="59.309" size="2" layer="95"/>
<attribute name="VALUE" x="245.999" y="54.737" size="2" layer="96"/>
</instance>
<instance part="RFB2" gate="A" x="244.475" y="77.47" smashed="yes" rot="R90">
<attribute name="NAME" x="245.999" y="80.264" size="2" layer="95"/>
<attribute name="VALUE" x="245.999" y="75.692" size="2" layer="96"/>
</instance>
<instance part="RSNS" gate="A" x="204.47" y="134.62" smashed="yes" rot="R90">
<attribute name="NAME" x="205.994" y="137.414" size="2" layer="95"/>
<attribute name="VALUE" x="205.994" y="132.842" size="2" layer="96"/>
</instance>
<instance part="RT" gate="A" x="106.045" y="109.22" smashed="yes" rot="R90">
<attribute name="NAME" x="107.569" y="112.014" size="2" layer="95"/>
<attribute name="VALUE" x="107.569" y="107.442" size="2" layer="96"/>
</instance>
<instance part="U1" gate="A" x="138.43" y="96.52" smashed="yes">
<attribute name="NAME" x="138.43" y="96.52" size="2" layer="95" align="bottom-center"/>
<attribute name="TYPE" x="138.43" y="92.71" size="2" layer="96" align="bottom-center"/>
</instance>
<instance part="CIN_2" gate="A" x="93.98" y="91.44" smashed="yes" rot="R90">
<attribute name="NAME" x="97.028" y="95.758" size="2" layer="95"/>
<attribute name="VALUE" x="97.028" y="91.186" size="2" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="FB" class="0">
<segment>
<pinref part="U1" gate="A" pin="FB"/>
<wire x1="244.475" y1="66.04" x2="230.505" y2="66.04" width="0" layer="91"/>
<pinref part="CFF" gate="A" pin="2"/>
<wire x1="230.505" y1="66.04" x2="163.83" y2="66.04" width="0" layer="91"/>
<wire x1="230.505" y1="69.85" x2="230.505" y2="66.04" width="0.1524" layer="91"/>
<junction x="230.505" y="66.04"/>
<pinref part="RFB2" gate="A" pin="2"/>
<wire x1="244.475" y1="69.85" x2="244.475" y2="66.04" width="0.1524" layer="91"/>
<pinref part="RFB1" gate="A" pin="1"/>
<wire x1="244.475" y1="64.135" x2="244.475" y2="66.04" width="0.1524" layer="91"/>
<junction x="244.475" y="66.04"/>
</segment>
</net>
<net name="ADJ" class="0">
<segment>
<pinref part="RADJ" gate="A" pin="2"/>
<pinref part="U1" gate="A" pin="ADJ"/>
<wire x1="163.83" y1="111.76" x2="167.005" y2="111.76" width="0" layer="91"/>
<pinref part="CADJ" gate="A" pin="2"/>
<wire x1="167.005" y1="111.76" x2="168.91" y2="111.76" width="0" layer="91"/>
<wire x1="167.005" y1="125.73" x2="167.005" y2="111.76" width="0.1524" layer="91"/>
<junction x="167.005" y="111.76"/>
</segment>
</net>
<net name="VCC_5085" class="0">
<segment>
<pinref part="CVCC" gate="A" pin="2"/>
<pinref part="U1" gate="A" pin="VCC"/>
<wire x1="163.83" y1="132.08" x2="163.83" y2="127" width="0" layer="91"/>
</segment>
</net>
<net name="9" class="0">
<segment>
<pinref part="D1" gate="A" pin="K"/>
<pinref part="L1" gate="A" pin="1"/>
<pinref part="M1" gate="A" pin="DRAIN_2"/>
<pinref part="M1" gate="A" pin="DRAIN_2"/>
<pinref part="M1" gate="A" pin="DRAIN_2"/>
<pinref part="M1" gate="A" pin="DRAIN_2"/>
<pinref part="M1" gate="A" pin="DRAIN_2"/>
<wire x1="204.47" y1="88.9" x2="204.47" y2="86.36" width="0" layer="91"/>
<wire x1="212.09" y1="86.36" x2="209.55" y2="86.36" width="0" layer="91"/>
<wire x1="209.55" y1="86.36" x2="204.47" y2="86.36" width="0" layer="91"/>
<wire x1="209.55" y1="85.09" x2="209.55" y2="86.36" width="0.1524" layer="91"/>
<junction x="209.55" y="86.36"/>
</segment>
</net>
<net name="PGATE" class="0">
<segment>
<pinref part="M1" gate="A" pin="GATE"/>
<pinref part="U1" gate="A" pin="PGATE"/>
<wire x1="163.83" y1="81.28" x2="196.215" y2="81.28" width="0" layer="91"/>
<wire x1="196.215" y1="81.28" x2="196.215" y2="96.52" width="0" layer="91"/>
<wire x1="196.85" y1="96.52" x2="196.215" y2="96.52" width="0" layer="91"/>
</segment>
</net>
<net name="ISEN" class="0">
<segment>
<pinref part="U1" gate="A" pin="ISEN"/>
<wire x1="163.83" y1="96.52" x2="191.135" y2="96.52" width="0" layer="91"/>
<wire x1="191.135" y1="96.52" x2="191.135" y2="121.92" width="0" layer="91"/>
<wire x1="191.135" y1="121.92" x2="204.47" y2="121.92" width="0" layer="91"/>
<pinref part="M1" gate="A" pin="SOURCE_2"/>
<pinref part="M1" gate="A" pin="SOURCE_2"/>
<pinref part="M1" gate="A" pin="SOURCE_2"/>
<wire x1="204.47" y1="104.14" x2="204.47" y2="121.92" width="0.1524" layer="91"/>
<junction x="204.47" y="104.14"/>
<pinref part="RSNS" gate="A" pin="2"/>
<wire x1="204.47" y1="121.92" x2="204.47" y2="127" width="0.1524" layer="91"/>
<junction x="204.47" y="121.92"/>
</segment>
</net>
<net name="RT" class="0">
<segment>
<pinref part="U1" gate="A" pin="RT"/>
<wire x1="106.045" y1="93.98" x2="113.03" y2="93.98" width="0" layer="91"/>
<pinref part="RT" gate="A" pin="2"/>
<wire x1="106.045" y1="101.6" x2="106.045" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="VOUT" class="0">
<segment>
<pinref part="L1" gate="A" pin="2"/>
<wire x1="227.33" y1="86.36" x2="230.505" y2="86.36" width="0" layer="91"/>
<pinref part="CFF" gate="A" pin="1"/>
<wire x1="230.505" y1="86.36" x2="244.475" y2="86.36" width="0" layer="91"/>
<wire x1="244.475" y1="86.36" x2="256.54" y2="86.36" width="0" layer="91"/>
<wire x1="256.54" y1="86.36" x2="279.4" y2="86.36" width="0" layer="91"/>
<wire x1="230.505" y1="85.09" x2="230.505" y2="86.36" width="0.1524" layer="91"/>
<junction x="230.505" y="86.36"/>
<pinref part="RFB2" gate="A" pin="1"/>
<wire x1="244.475" y1="85.09" x2="244.475" y2="86.36" width="0.1524" layer="91"/>
<junction x="244.475" y="86.36"/>
<pinref part="COUT" gate="A" pin="+"/>
<wire x1="256.54" y1="66.04" x2="256.54" y2="86.36" width="0.1524" layer="91"/>
<junction x="256.54" y="86.36"/>
</segment>
</net>
<net name="GND_5085" class="0">
<segment>
<pinref part="CIN_2" gate="A" pin="2"/>
<pinref part="CIN" gate="A" pin="2"/>
<wire x1="76.2" y1="83.82" x2="93.98" y2="83.82" width="0" layer="91"/>
<pinref part="U1" gate="A" pin="GND_2"/>
<pinref part="U1" gate="A" pin="GND_2"/>
<wire x1="113.03" y1="73.66" x2="76.2" y2="73.66" width="0" layer="91"/>
<wire x1="76.2" y1="83.82" x2="76.2" y2="73.66" width="0.1524" layer="91"/>
<junction x="76.2" y="83.82"/>
<wire x1="60.96" y1="63.5" x2="76.2" y2="63.5" width="0" layer="91"/>
<wire x1="76.2" y1="63.5" x2="76.2" y2="73.66" width="0.1524" layer="91"/>
<junction x="76.2" y="73.66"/>
<pinref part="D1" gate="A" pin="A_2"/>
<pinref part="D1" gate="A" pin="A_2"/>
<wire x1="104.14" y1="45.72" x2="209.55" y2="45.72" width="0" layer="91"/>
<wire x1="209.55" y1="45.72" x2="244.475" y2="45.72" width="0" layer="91"/>
<wire x1="244.475" y1="45.72" x2="256.54" y2="45.72" width="0" layer="91"/>
<wire x1="256.54" y1="45.72" x2="279.4" y2="45.72" width="0" layer="91"/>
<wire x1="209.55" y1="69.85" x2="209.55" y2="45.72" width="0.1524" layer="91"/>
<junction x="209.55" y="69.85"/>
<junction x="209.55" y="45.72"/>
<pinref part="RFB1" gate="A" pin="2"/>
<wire x1="244.475" y1="48.895" x2="244.475" y2="45.72" width="0.1524" layer="91"/>
<junction x="244.475" y="45.72"/>
<pinref part="COUT" gate="A" pin="-"/>
<wire x1="256.54" y1="50.8" x2="256.54" y2="45.72" width="0.1524" layer="91"/>
<junction x="256.54" y="45.72"/>
<wire x1="76.2" y1="63.5" x2="104.14" y2="63.5" width="0.1524" layer="91"/>
<wire x1="104.14" y1="63.5" x2="104.14" y2="45.72" width="0.1524" layer="91"/>
<junction x="76.2" y="63.5"/>
</segment>
</net>
<net name="VIN" class="0">
<segment>
<pinref part="RADJ" gate="A" pin="1"/>
<wire x1="184.15" y1="111.76" x2="185.42" y2="111.76" width="0" layer="91"/>
<pinref part="CADJ" gate="A" pin="1"/>
<wire x1="182.245" y1="125.73" x2="185.42" y2="125.73" width="0" layer="91"/>
<wire x1="185.42" y1="111.76" x2="185.42" y2="125.73" width="0.1524" layer="91"/>
<pinref part="CIN_2" gate="A" pin="1"/>
<pinref part="CIN" gate="A" pin="1"/>
<wire x1="76.2" y1="99.06" x2="93.98" y2="99.06" width="0" layer="91"/>
<pinref part="RSNS" gate="A" pin="1"/>
<wire x1="111.76" y1="152.4" x2="163.83" y2="152.4" width="0" layer="91"/>
<wire x1="163.83" y1="152.4" x2="185.42" y2="152.4" width="0" layer="91"/>
<wire x1="185.42" y1="152.4" x2="204.47" y2="152.4" width="0" layer="91"/>
<wire x1="204.47" y1="152.4" x2="204.47" y2="142.24" width="0" layer="91"/>
<pinref part="CVCC" gate="A" pin="1"/>
<wire x1="163.83" y1="147.32" x2="163.83" y2="152.4" width="0.1524" layer="91" style="longdash"/>
<junction x="163.83" y="152.4"/>
<pinref part="U1" gate="A" pin="VIN"/>
<wire x1="60.96" y1="119.38" x2="76.2" y2="119.38" width="0" layer="91"/>
<wire x1="76.2" y1="119.38" x2="106.045" y2="119.38" width="0" layer="91"/>
<wire x1="106.045" y1="119.38" x2="111.76" y2="119.38" width="0" layer="91"/>
<wire x1="111.76" y1="119.38" x2="113.03" y2="119.38" width="0" layer="91"/>
<wire x1="111.76" y1="152.4" x2="111.76" y2="119.38" width="0.1524" layer="91"/>
<junction x="111.76" y="119.38"/>
<pinref part="RT" gate="A" pin="1"/>
<wire x1="106.045" y1="116.84" x2="106.045" y2="119.38" width="0.1524" layer="91"/>
<junction x="106.045" y="119.38"/>
<wire x1="76.2" y1="99.06" x2="76.2" y2="119.38" width="0.1524" layer="91"/>
<junction x="76.2" y="99.06"/>
<junction x="76.2" y="119.38"/>
<wire x1="185.42" y1="125.73" x2="185.42" y2="152.4" width="0.1524" layer="91"/>
<junction x="185.42" y="125.73"/>
<junction x="185.42" y="152.4"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
