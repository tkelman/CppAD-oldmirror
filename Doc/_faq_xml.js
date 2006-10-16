var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_down1 = [
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
var list_current0 = [
'faq.xml#Assignment and Independent',
'faq.xml#Bugs',
'faq.xml#CompareChange',
'faq.xml#Complex Types',
'faq.xml#Exceptions',
'faq.xml#Independent Variables',
'faq.xml#Math Functions',
'faq.xml#Matrix Inverse',
'faq.xml#Mode: Forward or Reverse',
'faq.xml#Namespace',
'faq.xml#Namespace.CppADvector Preprocessor Symbol',
'faq.xml#Namespace.Using',
'faq.xml#Speed',
'faq.xml#Tape Storage: Disk or Memory'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
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
