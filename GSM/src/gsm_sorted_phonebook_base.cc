#ifdef HAVE_CONFIG_H
#include <gsm_config.h>
#endif
#include <gsm_sorted_phonebook_base.h>
#include <gsm_error.h>
#include <gsm_util.h>
#include <gsm_nls.h>

#include <assert.h>

using namespace std;
using namespace gsmlib;

// PhonebookEntryBase members

void PhonebookEntryBase::set(string telephone, string text, int index,
                             bool useIndex)
  throw(GsmException)
{
  checkTextAndTelephone(text, telephone);

  _changed = true;
  _telephone = telephone;
  _text = text;
  _useIndex = useIndex;
  if (index != -1)
    _index = index;
}

bool PhonebookEntryBase::operator==(const PhonebookEntryBase &e) const
{
  assert(! ((_useIndex || e._useIndex) &&
            (_index == -1 || e._index == -1)));
  return _telephone == e._telephone && _text == e._text &&
    (! (_useIndex || e._useIndex) || _index == e._index);
}

string PhonebookEntryBase::text() const throw(GsmException)
{
  return _text;
}

string PhonebookEntryBase::telephone() const throw(GsmException)
{
  return _telephone;
}

bool PhonebookEntryBase::empty() const throw(GsmException)
{
  return (text() == "") && (telephone() == "");
}

Ref<PhonebookEntryBase> PhonebookEntryBase::clone()
{
  Ref<PhonebookEntryBase> result = new PhonebookEntryBase(*this);
  return result;
}

PhonebookEntryBase::PhonebookEntryBase(const PhonebookEntryBase &e)
  throw(GsmException)
{
  set(e._telephone, e._text, e._index, e._useIndex);
}

PhonebookEntryBase &PhonebookEntryBase::operator=(const PhonebookEntryBase &e)
  throw(GsmException)
{
  set(e._telephone, e._text, e._index, e._useIndex);
  return *this;
}

// CustomPhonebookRegistry members

map<string, CustomPhonebookFactory*>
*CustomPhonebookRegistry::_factoryList = NULL;

void CustomPhonebookRegistry::
registerCustomPhonebookFactory(string backendName,
                               CustomPhonebookFactory *factory)
  throw(GsmException)
{
  if (_factoryList == NULL)
    _factoryList = new map<string, CustomPhonebookFactory*>;
  backendName = lowercase(backendName);
  if (_factoryList->find(backendName) != _factoryList->end())
    throw GsmException(stringPrintf(_("backend '%s' already registered"),
                                    backendName.c_str()), ParameterError);
}
      
SortedPhonebookRef CustomPhonebookRegistry::
createPhonebook(string backendName, string source) throw(GsmException)
{
  if (_factoryList == NULL)
    _factoryList = new map<string, CustomPhonebookFactory*>;
  backendName = lowercase(backendName);
  if (_factoryList->find(backendName) == _factoryList->end())
    throw GsmException(stringPrintf(_("backend '%s' not registered"),
                                    backendName.c_str()), ParameterError);
  return (*_factoryList)[backendName]->createPhonebook(source);
}
