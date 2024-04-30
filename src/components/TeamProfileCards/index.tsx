/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, {type ReactNode} from 'react';
import Translate from '@docusaurus/Translate';
import Link from '@docusaurus/Link';
import Heading from '@theme/Heading';

function WebsiteLink({to, children}: {to: string; children?: ReactNode}) {
  return (
    <Link to={to}>
      {children ?? (
        <Translate id="team.profile.websiteLinkLabel">website</Translate>
      )}
    </Link>
  );
}

type ProfileProps = {
  className?: string;
  name: string;
  children: ReactNode;
  githubUrl: string;
  twitterUrl?: string;
  emailId?:string;
  linkedIn?:string;
  website?:string;
};

function TeamProfileCard({
  className,
  name,
  children,
  githubUrl,
  twitterUrl,
  emailId,
  linkedIn,
  website,
}: ProfileProps) {
  return (
    <div className={className}>
      <div className="card card--full-height">
        <div className="card__header">
          <div className="avatar avatar--vertical">
            <img
              className="avatar__photo avatar__photo--xl"
              src={`img/${name}.png`}
              alt={`${name}'s avatar`}
            />
            <div className="avatar__intro">
              <Heading as="h3" className="avatar__name">
                {name}
              </Heading>
            </div>
          </div>
        </div>
        <div className="card__body">{children}</div>
        <div className="card__footer">
          <div className="button-group button-group--block">
            {githubUrl && (
              <Link className="button button--secondary" href={githubUrl}>
                GitHub
              </Link>
            )}
            {twitterUrl && (
              <Link className="button button--secondary" href={twitterUrl}>
                Twitter
              </Link>
            )}
            {linkedIn && (
              <Link className="button button--secondary" href={linkedIn}>
                Linked In
              </Link>
            )}
            {website && (
              <Link className="button button--secondary" href={website}>
                Website
              </Link>
            )}
            {emailId && (
              <Link className="button button--secondary" href={`mailto:${emailId}`}>
                E-Mail
              </Link>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}

function TeamProfileCardCol(props: ProfileProps) {
  return (
    <TeamProfileCard {...props} className="col col--6 margin-bottom--lg" />
  );
}

export function ActiveTeamRow(): JSX.Element {
  return (
    <div className="row">
      <TeamProfileCardCol
        name="Dr K V Gangadharan"
        linkedIn="https://in.linkedin.com/in/kvganga"
        website="https://mech.nitk.ac.in/faculty/k-v-gangadharan">
        <div style={{ textAlign: 'center' }}>
            <Translate
            id="team.profile.K V Gangadharan.body">
            Coordinator
            </Translate>
            <br />
            <Translate
            id="team.profile.K V Gangadharan.dept">
            Centre for System Design
            </Translate>
        </div>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Akhilesh Gonabal"
        githubUrl="https://github.com/akhilG05"
        emailId="akhilesh.gonabal@gmail.com">
        <div style={{ textAlign: 'center' }}>
            <Translate
            id="team.profile.Akhilesh Gonabal.body">
            Junior Research Fellow
            </Translate>
            <br />
            <Translate
            id="team.profile.Akhilesh Gonabal.dept">
            Centre for System Design
            </Translate>
        </div>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Neil Jose"
        emailId="neiljose0070@gmail.com">
        <div style={{ textAlign: 'center' }}>
            <Translate
            id="team.profile.Neil Jose.body">
            Junior Research Fellow
            </Translate>
            <br />
            <Translate
            id="team.profile.Neil Jose.dept">
            Centre for System Design
            </Translate>
        </div>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Jerit Jose"
        linkedIn="http://www.linkedin.com/in/jerit-jose-7991ba274"
        emailId="jeritjose17@gmail.com">
        <div style={{ textAlign: 'center' }}>
            <Translate
            id="team.profile.Jerit Jose.body">
            Junior Assistant
            </Translate>
            <br />
            <Translate
            id="team.profile.Jerit Jose.dept">
            Centre for System Design
            </Translate>
        </div>
      </TeamProfileCardCol>
    </div>
  );
}

export function HonoraryAlumniTeamRow(): JSX.Element {
  return (
    <div className="row">
      <TeamProfileCardCol
        name="Joel Marcey"
        githubUrl="https://github.com/JoelMarcey"
        twitterUrl="https://twitter.com/joelmarcey">
        <Translate id="team.profile.Joel Marcey.body">
          Docusaurus founder and now ever grateful Docusaurus cheerleader to
          those who actually write code for it.
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Alexey Pyltsyn"
        githubUrl="https://github.com/lex111">
        <Translate id="team.profile.Alexey Pyltsyn.body">
          Obsessed open-source enthusiast 👋 Eternal amateur at everything 🤷‍♂️
          Maintainer of Russian docs on PHP, React, Kubernetes and much more 🧐
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name=""
        githubUrl=""
        twitterUrl="">
        <Translate id="team.profile..body">
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name=""
        githubUrl=""
        twitterUrl="">
        <Translate id="team.profile..body">
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Wei Gao"
        githubUrl="https://github.com/wgao19"
        twitterUrl="https://twitter.com/wgao19">
        <Translate id="team.profile.Wei Gao.body">
          🏻‍🌾 Work in progress React developer, maintains Docusaurus, writes
          docs and spams this world with many websites.
        </Translate>
      </TeamProfileCardCol>
    </div>
  );
}

export function StudentFellowsTeamRow(): JSX.Element {
  return (
    <div className="row">
      <TeamProfileCardCol
        name="Anshul Goyal"
        githubUrl="https://github.com/anshulrgoyal"
        twitterUrl="https://twitter.com/ar_goyal">
        <Translate
          id="team.profile.Anshul Goyal.body"
          values={{
            websiteLink: (
              <Link href="https://anshulgoyal.dev/">
                <Translate id="team.profile.Anshul Goyal.body.websiteLink.label">
                  website
                </Translate>
              </Link>
            ),
          }}>
          {
            'Fullstack developer who loves to code and try new technologies. In his free time, he contributes to open source, writes blog posts on his {websiteLink} and watches Anime.'
          }
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Drew Alexander"
        githubUrl="https://github.com/drewbi">
        <Translate id="team.profile.Drew Alexander.body">
          Developer and Creative, trying to gain the skills to build whatever he
          can think of.
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Fanny Vieira"
        githubUrl="https://github.com/fanny"
        twitterUrl="https://twitter.com/fannyvieiira">
        <Translate
          id="team.profile.Fanny Vieira.body"
          values={{
            blogLink: (
              <Link href="https://dev.to/fannyvieira">
                <Translate id="team.profile.Fanny Vieira.body.blogLink.label">
                  her blog
                </Translate>
              </Link>
            ),
            spotifyLink: (
              <Link href="https://open.spotify.com/user/anotherfanny">
                <Translate id="team.profile.Fanny Vieira.body.spotifyLink.label">
                  Spotify playlists
                </Translate>
              </Link>
            ),
          }}>
          {
            'Fanny got started with web development in high school, building a project for the school kitchen. In her free time she loves contributing to Open Source, occasionally writing on {blogLink} about her experiences, cooking, and creating {spotifyLink}.'
          }
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Sam Zhou"
        githubUrl="https://github.com/SamChou19815"
        twitterUrl="https://twitter.com/SamChou19815">
        <Translate
          id="team.profile.Sam Zhou.body"
          values={{
            websiteLink: (
              <Link href="https://developersam.com">
                <Translate id="team.profile.Anshul Goyal.body.websiteLink.label">
                  website
                </Translate>
              </Link>
            ),
            samLangLink: (
              <Link href="https://samlang.developersam.com/">
                <Translate id="team.profile.Sam Zhou.body.samLangLink.label">
                  programming language
                </Translate>
              </Link>
            ),
            miniReactLink: (
              <Link href="https://github.com/SamChou19815/mini-react">
                <Translate id="team.profile.Sam Zhou.body.miniReactLink.label">
                  mini React
                </Translate>
              </Link>
            ),
          }}>
          {
            'Sam started programming in 2011 and built his {websiteLink} in 2015. He is interested in programming languages, dev infra and web development, and has built his own {samLangLink} and {miniReactLink}.'
          }
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Tan Teik Jun"
        githubUrl="https://github.com/teikjun"
        twitterUrl="https://twitter.com/teik_jun">
        <Translate id="team.profile.Tan Teik Jun.body">
          Open-source enthusiast who aims to become as awesome as the other
          humans on this page. Working on Docusaurus brought him closer to his
          goal. 🌱
        </Translate>
      </TeamProfileCardCol>
      <TeamProfileCardCol
        name="Nisarag Bhatt"
        githubUrl="https://github.com/FocalChord"
        twitterUrl="https://twitter.com/focalchord_">
        <Translate id="team.profile.Nisarag Bhatt.body">
          Fullstack web developer who loves learning new technologies and
          applying them! Loves contributing to open source as well as writing
          content articles and tutorials.
        </Translate>
      </TeamProfileCardCol>
    </div>
  );
}
