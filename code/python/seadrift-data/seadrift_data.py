#!/usr/bin/env python
# coding: utf-8

# In[1]:


import imaplib
import email
from email.header import decode_header
import webbrowser
import os
import schedule  
import time 

def clean(text):
    # clean text for creating a folder
    return "".join(c if c.isalnum() else "_" for c in text)

def addzero(ori):
    new = str()
    for i in range(8-len(ori)):
        new += '0'
    new += ori
    return new

def decoder(oridata,timedata):
    data=open("C:\\Users\\Nelson\\Desktop\\sea_drift_data.txt",'w+')
    
    newtimedata = timedata[0:10]+' '+timedata[11:13]+':'+timedata[13:15]+':'+timedata[15:17]+' '+timedata[19:22]
    print(newtimedata,file=data)
    print(newtimedata)
    
    full = str()
    
    bytes_num = int(len(oridata)/2)
    #print(bytes_num)
    
    #oridata = '084081620193000200022650660bc00548c022039ae4cc0bd282f0000000'
    
    for i in range(bytes_num-1):
        temp = bin(int(oridata[2*i:2*(i+1)],16))
        full += addzero(temp[2:])
        #print(full)
    
    if full[0] == '0':
        accx = int(full[1:10],2) / 100
    else:
        accx = int(full[1:10],2) / 100 * -1
        
    if full[10] == '0':
        accy = int(full[11:20],2) / 100
    else:
        accy = int(full[11:20],2) /100 * -1
        
    if full[20] == '0':
        accz = int(full[21:30],2) / 100
    else:
        accz = int(full[21:30],2) / 100 * -1
    
    print(accx,file=data)
    print(accy,file=data)
    print(accz,file=data)
    print(accx,accy,accz)
    
    if full[30] == '0':
        gyrox = int(full[31:47],2) / 100
    else:
        gyrox = int(full[31:47],2) / 100 * -1
        
    if full[47] == '0':
        gyroy = int(full[48:64],2) / 100
    else:
        gyroy = int(full[48:64],2) / 100 * -1
        
    if full[64] == '0':
        gyroz = int(full[65:81],2) / 100
    else:
        gyroz = int(full[65:81],2) / 100 * -1
        
    print(gyrox,file=data)
    print(gyroy,file=data)
    print(gyroz,file=data)
    print(gyrox,gyroy,gyroz)
    
    if full[81] == '0':
        pressure = int(full[82:106],2) / 100
    else:
        pressure = int(full[82:106],2) / 100 * -1
    
    print(pressure,file=data)
    print(pressure)
    
    if full[106] == '0':
        temper = int(full[107:120],2) / 100
    else:
        temper = int(full[107:120],2) / 100 * -1
    
    print(temper,file=data)
    print(temper)
    
    if full[120] == '0':
        height = int(full[121:138],2) / 100
    else:
        height = int(full[121:138],2) / 100 * -1
        
    print(height,file=data)
    print(height)
    
    if full[138] == '0':
        wave_height = int(full[139:156],2) / 100
    else:
        wave_height = int(full[139:156],2) / 100 * -1
        
    print(wave_height,file=data)
    print(wave_height)
    
    if full[156] == '0':
        longitute = int(full[157:185],2) / 1000000
    else:
        longitute = int(full[157:185],2) / 1000000 * -1
    
    print(longitute,file=data)
    print(longitute)
    
    if full[185] == '0':
        latitute = int(full[186:213],2) / 1000000
    else:
        latitute = int(full[186:213],2) / 1000000 * -1
    
    print(latitute,file=data)
    print(latitute)
    data.close()
    
    return
    
def work():
    # account credentials
    username = os.getenv("SEADRIFT_EMAIL_USER")
    password = os.getenv("SEADRIFT_EMAIL_PASSWORD")
#     username = os.getenv("SEADRIFT_ALT_EMAIL_USER")
#     password = os.getenv("SEADRIFT_ALT_EMAIL_PASSWORD")
#     use your email provider's IMAP server, you can look for your provider's IMAP server on Google
#     or check this page: https://www.systoolsgroup.com/imap/
#     for office 365, it's this:
#     imap_server = "outlook.office365.com" #for office365 mailbox
    imap_server = "imap-mail.outlook.com" #for outlook mailbox
    
    # create an IMAP4 class with SSL 
    imap = imaplib.IMAP4_SSL(imap_server)
    # authenticate
    imap.login(username, password)

    status, messages = imap.select("INBOX")
    # number of top emails to fetch
    N = 1
    # total number of emails
    messages = int(messages[0])
    
    for i in range(messages, messages-N, -1):
        # fetch the email message by ID
        res, msg = imap.fetch(str(i), "(RFC822)")
        for response in msg:
            if isinstance(response, tuple):
                #print('here is if')
                # parse a bytes email into a message object
                msg = email.message_from_bytes(response[1])
                # decode the email subject
                subject, encoding = decode_header(msg["Subject"])[0]
                if isinstance(subject, bytes):
                    # if it's a bytes, decode to str
                    subject = subject.decode(encoding)
                # decode email sender
                From, encoding = decode_header(msg.get("From"))[0]
                if isinstance(From, bytes):
                    From = From.decode(encoding)
                #print("Subject:", subject)
                print("From:", From)
                if os.getenv("ROCKBLOCK_IMEI", "") not in From:
                    print("Not Rockblock mail.")
                    return
                # if the email message is multipart
                if msg.is_multipart():
                    # iterate over email parts
                    for part in msg.walk():
                        # extract content type of email
                        content_type = part.get_content_type()
                        content_disposition = str(part.get("Content-Disposition"))
                        try:
                            # get the email body
                            body = part.get_payload(decode=True).decode()
                        except:
                            pass
                        if content_type == "text/plain" and "attachment" not in content_disposition:
                            # print text/plain emails and skip attachments
                            print(body)
                            temp = body.split(':')
                            #print(temp[-1])
                            keydata = temp[-1][1:-2]
                            print("The key data is:",end = '')
                            print(keydata)
                            timedata = temp[3][1:]+temp[4]+temp[5]
                            decoder(keydata,timedata)
                        elif "attachment" in content_disposition:
                            # download attachment
                            print('attachment')
                            #We dont download in this project

    #                         filename = part.get_filename()
    #                         if filename:
    #                             folder_name = clean(subject)
    #                             if not os.path.isdir(folder_name):
    #                                 # make a folder for this email (named after the subject)
    #                                 os.mkdir(folder_name)
    #                             filepath = os.path.join(folder_name, filename)
    #                             # download attachment and save it
    #                             open(filepath, "wb").write(part.get_payload(decode=True))

                else:
                    #print('Here is else')
                    # extract content type of email
                    content_type = msg.get_content_type()
                    # get the email body
                    body = msg.get_payload(decode=True).decode()
                    if content_type == "text/plain":
                        # print only text email parts
                        print('content type is plain')
                        print(body)
                if content_type == "text/html":
                    # if it's HTML, create a new HTML file and open it in browser
                    folder_name = clean(subject)
                    if not os.path.isdir(folder_name):
                        # make a folder for this email (named after the subject)
                        os.mkdir(folder_name)
                    filename = "index.html"
                    filepath = os.path.join(folder_name, filename)
                    # write the file
                    open(filepath, "w").write(body)
                    # open in the default browser
                    #webbrowser.open(filepath) #we dont do this, too
                print("="*100)
    # close the connection and logout
    imap.close()
    imap.logout() 
    return

schedule.every(0.1).minutes.do(work)  
# schedule.every().hour.do(job)  
# schedule.every().day.at("10:30").do(job)  
# schedule.every().monday.do(job)  
# schedule.every().wednesday.at("13:15").do(job)  
  
while True:  
    schedule.run_pending()
    print("Waiting...")
    time.sleep(1)  

