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
'errorhandler.xml',
'nearequal.xml',
'speedtest.xml',
'numerictype.xml',
'checknumerictype.xml',
'simplevector.xml',
'checksimplevector.xml',
'powint.xml',
'poly.xml',
'ludetandsolve.xml',
'rombergone.xml',
'rombergmul.xml',
'runge45.xml',
'rosen34.xml',
'odeerrcontrol.xml',
'odegear.xml',
'odegearcontrol.xml',
'benderquad.xml',
'luratio.xml',
'cppad_vector.xml',
'tracknewdel.xml'
];
var list_down1 = [
'lusolve.xml',
'lusolve.h.xml',
'lufactor.xml',
'lufactor.h.xml',
'luinvert.xml',
'luinvert.h.xml'
];
var list_down0 = [
'lufactor.cpp.xml'
];
var list_current0 = [
'lufactor.xml#Syntax',
'lufactor.xml#Description',
'lufactor.xml#Include',
'lufactor.xml#Matrix Storage',
'lufactor.xml#sign',
'lufactor.xml#ip',
'lufactor.xml#jp',
'lufactor.xml#LU',
'lufactor.xml#LU.A',
'lufactor.xml#LU.P',
'lufactor.xml#LU.L',
'lufactor.xml#LU.U',
'lufactor.xml#LU.Factor',
'lufactor.xml#LU.Determinant',
'lufactor.xml#SizeVector',
'lufactor.xml#FloatVector',
'lufactor.xml#Float',
'lufactor.xml#AbsGeq',
'lufactor.xml#Example'
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
