var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_down2 = [
'license.htm',
'faq.htm',
'install.htm',
'getstarted.cpp.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'library.htm',
'example.htm',
'appendix.htm'
];
var list_down1 = [
'errorhandler.htm',
'nearequal.htm',
'speedtest.htm',
'numerictype.htm',
'checknumerictype.htm',
'simplevector.htm',
'checksimplevector.htm',
'powint.htm',
'poly.htm',
'ludetandsolve.htm',
'rombergone.htm',
'rombergmul.htm',
'runge45.htm',
'rosen34.htm',
'odeerrcontrol.htm',
'odegear.htm',
'odegearcontrol.htm',
'benderquad.htm',
'luratio.htm',
'cppad_vector.htm',
'tracknewdel.htm'
];
var list_down0 = [
'speedexample.cpp.htm'
];
var list_current0 = [
'speedtest.htm#Syntax',
'speedtest.htm#Purpose',
'speedtest.htm#Repeat Factor',
'speedtest.htm#Include',
'speedtest.htm#Test',
'speedtest.htm#Test.size',
'speedtest.htm#Test.repeat',
'speedtest.htm#Test.name',
'speedtest.htm#first',
'speedtest.htm#last',
'speedtest.htm#inc',
'speedtest.htm#rate',
'speedtest.htm#Errors',
'speedtest.htm#Example'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
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
