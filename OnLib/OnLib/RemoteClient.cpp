#include "RemoteClient.h"
#include <iostream>

RemoteClient::RemoteClient(QObject* parent) :
	net::Client(), QObject(parent)
{
	log = [](LogMessageType type, const std::string& msg)
	{
		std::cout << msg;
	};

	processingThread = std::thread([this]()
		{
			while (true)
			{
				try
				{
					ProcessMessages(-1, true);
				}
				catch (std::exception& e)
				{
					//emit ExceptionThrown(e);
				}
			}
		});
}

void RemoteClient::OnLoginRequest(const data::User& user)
{
	net::Message msg;
	msg.header.messageType = data::EnumToNumber(data::ClientRequest::Login);
	net::Serialize(msg, user);
	Send(msg);
}

void RemoteClient::OnRegisterRequest(const data::User& user)
{
	net::Message msg;
	msg.header.messageType = data::EnumToNumber(data::ClientRequest::Register);
	net::Serialize(msg, user);
	Send(msg);
}

void RemoteClient::OnLogoutRequest()
{
	net::Message msg;
	msg.header.messageType = data::EnumToNumber(data::ClientRequest::Logout);
	Send(msg);
}

void RemoteClient::OnDeleteAccountRequest(const std::string& password)
{
	net::Message msg;
	msg.header.messageType = data::EnumToNumber(data::ClientRequest::DeleteAccount);
	net::Serialize(msg, std::cbegin(password), std::cend(password));
	Send(msg);
}

void RemoteClient::OnSearchRequest(const std::string& search)
{
	net::Message msg;
	msg.header.messageType = data::EnumToNumber(data::ClientRequest::SearchBooks);
	net::Serialize(msg, std::cbegin(search), std::cend(search));
	Send(msg);
}

void RemoteClient::OnRefreshRequest()
{
	RequestDisplayBooks();
	RequestBorrowedBooks();
}

void RemoteClient::OnBookRated(int rating, uint64_t bookId)
{
	net::Message rateRequestMsg;
	rateRequestMsg.header.messageType = data::EnumToNumber(data::ClientRequest::RateBook);
	rateRequestMsg << rating << bookId;
	Send(rateRequestMsg);
}

void RemoteClient::OnBookBorrowRequest(uint64_t bookId)
{
	net::Message borrowRequest;
	borrowRequest.header.messageType = data::EnumToNumber(data::ClientRequest::BorrowBook);
	borrowRequest << bookId;
	Send(borrowRequest);
}

void RemoteClient::OnBookReturnRequest(uint64_t bookId)
{
	net::Message returnRequest;
	returnRequest.header.messageType = data::EnumToNumber(data::ClientRequest::ReturnBook);
	returnRequest << bookId;
	Send(returnRequest);
}

RemoteClient::~RemoteClient()
{
	if (processingThread.joinable())
		processingThread.join();
}

void RemoteClient::RequestDisplayBooks()
{
	net::Message requestBooksMessage;
	requestBooksMessage.header.messageType = data::EnumToNumber(data::ClientRequest::RequestDisplayBooks);
	Send(requestBooksMessage);
}

void RemoteClient::RequestBorrowedBooks()
{
	net::Message requestBooksMessage;
	requestBooksMessage.header.messageType = data::EnumToNumber(data::ClientRequest::RequestLentBooks);
	Send(requestBooksMessage);
}

void RemoteClient::Send(const net::Message& msg) const
{
	if (IsConnected())
	{
		net::Client::Send(msg);
		return;
	}

	emit ConnectionLost();
}

void RemoteClient::OnMessageRecieved(net::Message& msg)
{
	data::ServerResponse response = static_cast<data::ServerResponse>(msg.header.messageType);

	switch (response)
	{
	case data::ServerResponse::ValidationSuccessfull:
		break;
	case data::ServerResponse::SuccesfullLogin:
	{
		emit LoginSuccessfull();
		break;
	}
	case data::ServerResponse::InvalidLogin:
	{
		std::vector<data::LoginErrors> errors;
		net::Deserialize(msg, errors, true);
		emit LoginInvalid(errors);
		break;
	}
	case data::ServerResponse::SuccesfullRegister:
		emit RegisterSuccesfull();
		break;
	case data::ServerResponse::InvalidRegister:
	{
		std::vector<data::RegisterErrors> errors;
		net::Deserialize(msg, errors, true);
		emit RegisterInvalid(errors);
		break;
	}
	case data::ServerResponse::DisplayBooksRecieved:
	{
		std::vector<data::Book> books;
		net::Deserialize(msg, books, true);
		emit DisplayBooksRecieved(books);
		break;
	}
	case data::ServerResponse::BorrowedBooksRecieved:
	{
		std::vector<data::LendBook> books;
		net::Deserialize(msg, books, true);
		emit BorrowedBooksRecieved(books);
		break;
	}
	case data::ServerResponse::DeleteAccountSuccesful:
		emit AccountDeleteSuccess();
		break;
	case data::ServerResponse::DeleteAccountFailure:
		emit AccountDeleteFailure();
		break;
	case data::ServerResponse::SearchedBooksRecieved:
	{
		std::vector<data::Book> books;
		net::Deserialize(msg, books, true);
		emit SearchedBooksRecieved(books);
		break;
	}
	default:
		break;
	}
}

void RemoteClient::ProcessMessages(std::size_t maxCount, bool sleep)
{
	if (sleep)
		messagesIn.Wait();

	std::size_t processed = 0;
	while (processed < maxCount && !messagesIn.Empty())
	{
		auto message = messagesIn.PopFront();
		OnMessageRecieved(message.message);
		processed++;
	}
}
