#include <iostream>
#include <fstream>


#include <PinFlds.h>
#include <PinContext.h>
#include <PinFlist.h>
#include <PinPoid.h>
#include <PinExc.h>
#include <PinLogger.h>

#include <pin_bill.h>
#include <pin_currency.h>
#include <pin_cust.h>
#include <pin_pymt.h>
#include <pin_rate.h>
#include <sstream>


using namespace std; 
std::string readFile()
{
	std::stringstream str1;
	ifstream stream("input.flist");
	if(stream.is_open())
	{
		while(stream.peek() != EOF)
		{
			str1 << (char) stream.get();
		}
		stream.close();
		return str1.str();
	}
}

ostream& operator<<(ostream&,PinEbufExc&);

void callOpcode(PinContextBase &context);


int main (int argc, char *argv[]) {

	try {

		cout << "Creating PCM context" << endl;
		PinContextOwner context = PinContext::create();
		callOpcode(context);


	} catch (PinEbufExc &exc) {
	    cout << "Connection to the server failed." << endl;
		cout << exc << endl;
	}
	return 0;
}


static void
init_random2(pin_err_t *errp)
{
    if (PIN_ERR_NONE != *errp)
    {
        return;
    }

    if (-1 == (tp_dev_urandom_fd = open("/dev/urandom", O_RDONLY)))
    {
        *errp = PIN_ERR_FILE_IO;
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
            "(): init failed; could not open /dev/urandom");
    }
}

void callOpcode(PinContextBase &context)
{
	PinFlistOwner	input = PinFlist::create();

	PinPoidDb	dbno = (PinPoidDb) 1;
	PinPoidOwner	poid = PinPoid::create(dbno, "/dummy", -1);

	input->set(tsf_PIN_FLD_POID, poid);

	PinEnum actionMode = 3;
	input->set(tsf_PIN_FLD_ACTION_MODE, actionMode);

	PinFlistOwner output;
	context->op(PCM_OP_TEST_LOOPBACK, 0, input, output);
	
	cout << "Context successfully created." << endl;

	pin_flist_t **r_flistpp;
	pin_flist_t *flist = output->get(); r_flistpp = &flist;
	cm_nap_connection_t	*connp = new cm_nap_connection_t;
	pcm_context_t *cntx = context->get();
	connp->dm_ctx = cntx;

	PinErrorBuf errorBuf;
	pin_errbuf_t *ebufp = errorBuf.get();
	pin_err_t ss;

	update_all_shm_config_cache(cntx, ebufp);
	std::string fileContent = readFile();
	cout << fileContent << endl;

	PinFlistOwner inputList = PinFlist::createFromString(fileContent.c_str());
	pin_flist_t *i_flistp2 = inputList->get();

	cout << endl << i_flistp2 << endl;
	init_random(&ss);

	pin_err_set_level(3);
	time_t t=1457046001;

	pin_virtual_time(&t);

        
        // calling opcode shoud be here

	PinFlistObserver outputList(*r_flistpp);
	cout << endl << outputList << endl;

}

ostream& operator<<(ostream &os, PinEbufExc &exc) {
	os << "Pin Exception";
	os << exc.getFlistRef() << endl;
	PIN_LOG(exc, PIN_ERR_LEVEL_ERROR, "");
	return os;
}


