var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_down3 = [
'license.xml',
'faq.xml',
'install.xml',
'getstarted.cpp.xml',
'introduction.xml',
'ad.xml',
'adfun.xml',
'library.xml',
'example.xml',
'appendix.xml'
];
var list_down2 = [
'general.xml',
'exampleutility.xml',
'listallexamples.xml',
'cppadvector.xml'
];
var list_down1 = [
'interface2c.cpp.xml',
'jacminordet.cpp.xml',
'jacludet.cpp.xml',
'hesminordet.cpp.xml',
'hesludet.cpp.xml',
'odestiff.cpp.xml',
'odetaylor.cpp.xml',
'complexpoly.cpp.xml',
'multape.xml'
];
var list_down0 = [
'multape.cpp.xml'
];
var list_current0 = [
'multape.xml#Background',
'multape.xml#Motivation',
'multape.xml#General Solution',
'multape.xml#General Procedure',
'multape.xml#General Procedure.Start ADBaseTape',
'multape.xml#General Procedure.Start ADDBaseTape',
'multape.xml#General Procedure.Inner Function Calculations',
'multape.xml#General Procedure.Derivative of Inner Function',
'multape.xml#General Procedure.Outer Function',
'multape.xml#General Procedure.Derivative of Outer Function',
'multape.xml#Example'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
